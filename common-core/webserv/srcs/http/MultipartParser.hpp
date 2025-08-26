#ifndef MULTIPART_PARSER_HPP
#define MULTIPART_PARSER_HPP

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "http_utils.hpp"

class MultipartParser {
   public:
    enum State {
        PARSING_BOUNDARY,
        PARSING_HEADERS,
        PARSING_BODY,
        COMPLETE,
        ERROR_STATE
    };

    struct FileInfo {
        std::string name;
        std::string filename;
        std::string content_type;
        std::string saved_path;
        size_t      size;
    };

   public:
    MultipartParser(const std::string& boundary, const std::string& upload_dir);
    ~MultipartParser();

    bool parseChunk(const std::string& chunk);
    bool isComplete() const;
    bool hasError() const;

    const std::vector<FileInfo>&              getFiles() const;
    const std::map<std::string, std::string>& getFormFields() const;

    std::string getErrorMessage() const;
    StatusCode  getStatusCode() const;

   private:
    std::string boundary_;
    std::string upload_dir_;
    std::string buffer_;
    State       state_;
    std::string error_message_;

    std::string   current_name_;
    std::string   current_filename_;
    std::string   current_content_type_;
    std::ofstream current_file_;
    std::string   current_file_path_;
    std::string   current_field_value_;

    std::vector<FileInfo>              files_;
    std::map<std::string, std::string> form_fields_;

    StatusCode status_code_;

    bool        parseHeaders(const std::string& headers_block);
    bool        extractContentDisposition(const std::string& header_value);
    std::string generateFilePath(const std::string& filename);
    bool        finalizCurrentPart();
    void        setState(State new_state);
    void        setError(const std::string& message);
    std::string trim(const std::string& str);
};

#endif
