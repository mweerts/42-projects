#include "MultipartParser.hpp"

#include <sys/stat.h>
#include <unistd.h>

#include <cstdlib>
#include <ctime>
#include <sstream>

#include "Logger.hpp"
#include "lib/file_utils.hpp"

MultipartParser::MultipartParser(const std::string& boundary,
                                 const std::string& upload_dir)
    : boundary_("--" + boundary),
      upload_dir_(upload_dir),
      state_(PARSING_BOUNDARY) {
    if (!lib::pathExist(upload_dir_)) {
        if (mkdir(upload_dir_.c_str(), 0755) != 0) {
            setError("Failed to create upload directory");
            return;
        }
    }

    Logger::debug() << "MultipartParser initialized with boundary: "
                    << boundary_;
}

MultipartParser::~MultipartParser() {
    if (current_file_.is_open()) {
        current_file_.close();
    }
}

bool MultipartParser::parseChunk(const std::string& chunk) {
    if (state_ == ERROR_STATE || state_ == COMPLETE) {
        return state_ == COMPLETE;
    }

    buffer_ += chunk;

    while (state_ != ERROR_STATE && state_ != COMPLETE && !buffer_.empty()) {
        switch (state_) {
            case PARSING_BOUNDARY: {
                size_t boundary_pos = buffer_.find(boundary_);
                if (boundary_pos == std::string::npos) {
                    if (buffer_.size() > boundary_.size()) {
                        buffer_ =
                            buffer_.substr(buffer_.size() - boundary_.size());
                    }
                    return true;
                }

                size_t after_boundary = boundary_pos + boundary_.size();
                if (after_boundary + 1 < buffer_.size() &&
                    buffer_.substr(after_boundary, 2) == "--") {
                    finalizCurrentPart();
                    setState(COMPLETE);
                    return true;
                }

                if (!current_name_.empty()) {
                    finalizCurrentPart();
                }

                buffer_ = buffer_.substr(after_boundary);
                setState(PARSING_HEADERS);
                break;
            }

            case PARSING_HEADERS: {
                size_t headers_end = buffer_.find("\r\n\r\n");
                if (headers_end == std::string::npos) {
                    headers_end = buffer_.find("\n\n");
                    if (headers_end == std::string::npos) {
                        return true;
                    }
                    headers_end += 2;
                } else {
                    headers_end += 4;
                }

                std::string headers_block = buffer_.substr(0, headers_end - 2);
                if (!parseHeaders(headers_block)) {
                    return false;
                }

                buffer_ = buffer_.substr(headers_end);
                setState(PARSING_BODY);
                break;
            }

            case PARSING_BODY: {
                size_t next_boundary = buffer_.find(boundary_);
                if (next_boundary == std::string::npos) {
                    size_t process_size = buffer_.size();
                    if (process_size > boundary_.size()) {
                        process_size = buffer_.size() - boundary_.size();
                    } else {
                        return true;
                    }

                    std::string body_chunk = buffer_.substr(0, process_size);
                    buffer_ = buffer_.substr(process_size);

                    if (!current_filename_.empty()) {
                        if (!current_file_.is_open()) {
                            current_file_path_ =
                                upload_dir_ + "/" + current_filename_;

                            current_file_.open(
                                current_file_path_.c_str(),
                                std::ios::binary | std::ios::out);
                            if (!current_file_.is_open()) {
                                setError("Failed to create file: " +
                                         current_file_path_);
                                return false;
                            }
                        }
                        current_file_.write(body_chunk.c_str(),
                                            body_chunk.size());
                    } else {
                        // C'est un champ de formulaire
                        current_field_value_ += body_chunk;
                    }
                    return true;
                } else {
                    std::string body_chunk = buffer_.substr(0, next_boundary);

                    if (body_chunk.size() >= 2 &&
                        body_chunk.substr(body_chunk.size() - 2) == "\r\n") {
                        body_chunk =
                            body_chunk.substr(0, body_chunk.size() - 2);
                    } else if (body_chunk.size() >= 1 &&
                               body_chunk[body_chunk.size() - 1] == '\n') {
                        body_chunk =
                            body_chunk.substr(0, body_chunk.size() - 1);
                    }

                    if (!current_filename_.empty()) {
                        if (!current_file_.is_open()) {
                            current_file_path_ =
                                upload_dir_ + "/" + current_filename_;
                            current_file_.open(
                                current_file_path_.c_str(),
                                std::ios::binary | std::ios::out);
                            if (!current_file_.is_open()) {
                                setError("Failed to create file: " +
                                         current_file_path_);
                                return false;
                            }
                        }
                        current_file_.write(body_chunk.c_str(),
                                            body_chunk.size());
                    } else {
                        current_field_value_ += body_chunk;
                    }

                    buffer_ = buffer_.substr(next_boundary);
                    setState(PARSING_BOUNDARY);
                }
                break;
            }

            default: return false;
        }
    }

    return true;
}

bool MultipartParser::parseHeaders(const std::string& headers_block) {
    current_name_.clear();
    current_filename_.clear();
    current_content_type_.clear();
    current_field_value_.clear();

    std::istringstream iss(headers_block);
    std::string        line;

    while (std::getline(iss, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line.erase(line.size() - 1);
        }
        if (line.empty())
            continue;
        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos)
            continue;

        std::string header_name = trim(line.substr(0, colon_pos));
        std::string header_value = trim(line.substr(colon_pos + 1));

        if (header_name == "Content-Disposition") {
            if (!extractContentDisposition(header_value)) {
                setError("Invalid Content-Disposition header");
                return false;
            }
        } else if (header_name == "Content-Type") {
            current_content_type_ = header_value;
        }
    }

    if (current_name_.empty()) {
        setError("Missing name in Content-Disposition");
        return false;
    }

    return true;
}

bool MultipartParser::extractContentDisposition(
    const std::string& header_value) {
    size_t name_pos = header_value.find("name=\"");
    if (name_pos != std::string::npos) {
        name_pos += 6;  // Longueur de "name=\""
        size_t name_end = header_value.find("\"", name_pos);
        if (name_end != std::string::npos) {
            current_name_ = header_value.substr(name_pos, name_end - name_pos);
        }
    }

    size_t filename_pos = header_value.find("filename=\"");
    if (filename_pos != std::string::npos) {
        filename_pos += 10;  // Longueur de "filename=\""
        size_t filename_end = header_value.find("\"", filename_pos);
        if (filename_end != std::string::npos) {
            current_filename_ =
                header_value.substr(filename_pos, filename_end - filename_pos);
        }
    }

    return !current_name_.empty();
}

std::string MultipartParser::generateFilePath(const std::string& filename) {
    std::ostringstream oss;
    oss << upload_dir_ << "/" << time(NULL) << "_" << filename;
    return oss.str();
}

bool MultipartParser::finalizCurrentPart() {
    if (current_name_.empty()) {
        return true;
    }

    if (!current_filename_.empty()) {
        if (current_file_.is_open()) {
            current_file_.close();

            struct stat st;
            size_t      file_size = 0;
            if (stat(current_file_path_.c_str(), &st) == 0) {
                file_size = st.st_size;
            }

            FileInfo file_info;
            file_info.name = current_name_;
            file_info.filename = current_filename_;
            file_info.content_type = current_content_type_;
            file_info.saved_path = current_file_path_;
            file_info.size = file_size;

            files_.push_back(file_info);

            Logger::info() << "File uploaded: " << current_filename_ << " -> "
                           << current_file_path_ << " (" << file_size
                           << " bytes)";
        }
    } else {
        form_fields_[current_name_] = current_field_value_;
        Logger::debug() << "Form field: " << current_name_ << " = "
                        << current_field_value_;
    }

    current_name_.clear();
    current_filename_.clear();
    current_content_type_.clear();
    current_field_value_.clear();
    current_file_path_.clear();

    return true;
}

void MultipartParser::setState(State new_state) {
    state_ = new_state;
}

void MultipartParser::setError(const std::string& message) {
    error_message_ = message;
    state_ = ERROR_STATE;
    Logger::error() << "MultipartParser error: " << message;
}

std::string MultipartParser::trim(const std::string& str) {
    size_t start = 0;
    size_t end = str.length();

    while (start < end && (str[start] == ' ' || str[start] == '\t')) {
        start++;
    }

    while (end > start && (str[end - 1] == ' ' || str[end - 1] == '\t')) {
        end--;
    }

    return str.substr(start, end - start);
}

bool MultipartParser::isComplete() const {
    return state_ == COMPLETE;
}

bool MultipartParser::hasError() const {
    return state_ == ERROR_STATE;
}

const std::vector<MultipartParser::FileInfo>& MultipartParser::getFiles()
    const {
    return files_;
}

const std::map<std::string, std::string>& MultipartParser::getFormFields()
    const {
    return form_fields_;
}

std::string MultipartParser::getErrorMessage() const {
    return error_message_;
}
