#include "RequestHandler.hpp"

#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>

#include "../parsing/GlobalConfig.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Logger.hpp"
#include "MimeTypes.hpp"
#include "MultipartParser.hpp"
#include "http_status_code.hpp"
#include "utils.hpp"

// TODO create upload dir if not exists

RequestHandler::RequestHandler(const HttpRequest&  request,
                               const ServerConfig& serverConfig)
    : _serverConfig(serverConfig),
      _request(request),
      _rootPath(serverConfig.getRoot()),
      _autoindex(serverConfig.getAutoIndex()) {};

RequestHandler::~RequestHandler() {}

// void RequestHandler::setServerConfig(const ServerConfig& config) {
//     _ServerConfig = config;
// }

void RequestHandler::setResponse(const HttpResponse& response) {
    _response = response;
}

const std::string RequestHandler::getRootPath() const {
    return _rootPath;
}

static std::string getLastModifiedTime(const std::string& path) {
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) == 0) {
        char       buffer[80];
        struct tm* tm_info = gmtime(&fileInfo.st_mtime);
        strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", tm_info);
        return std::string(buffer);
    }
    return "";
}

bool RequestHandler::shouldCloseConnection() const {
    if (_response.getStatusCode() != HTTP_OK) {
        return true;
    }

    return !_request.shouldKeepAlive();
}

void RequestHandler::handleRequest() {
    processRequest();

    if (_request.shouldKeepAlive()) {
        _response.setConnection("keep-alive");
    } else {
        _response.setConnection("close");
    }

    if (_response.getStatusCode() != HTTP_OK &&
        _response.getStatusCode() != HTTP_MOVED_PERMANENTLY) {
        std::ostringstream oss;
        oss << _response.getStatusCode();
        const std::string* errorPage = _serverConfig.getErrorPage(oss.str());
        // Logger::debug() << "ERROR PAGE :" + *errorPage + "  " + _rootPath;
        if (errorPage) {
            std::ifstream file((_rootPath + "/" + *errorPage).c_str());
            if (file.fail()) {
                _response.setContent(GetHtmlErrorPage(_response));
                _response.setContentType("text/html");
                return;
            }
            std::ostringstream ss;
            ss << file.rdbuf();
            _response.setContent(ss.str());
            _response.setContentType(
                MimeTypes::getType((_rootPath + "/" + *errorPage).c_str()));
            _response.setLastModified(
                getLastModifiedTime(_rootPath + "/" + *errorPage));
        } else if (_response.getStatusCode() != HTTP_NO_CONTENT) {
            _response.setContent(GetHtmlErrorPage(_response));
            _response.setContentType("text/html");
        }
        _response.setConnection("close");
    }
}

void RequestHandler::sendResponse(int socket_fd) {
    std::string responseString = _response.toString();
    Logger::debug() << "Sending response...";
    Logger::debug() << responseString;
    send(socket_fd, responseString.c_str(), responseString.size(), 0);
}

void RequestHandler::processRequest() {
    const Location*    location = _serverConfig.getLocation(_request.getUri());
    const std::string& method = _request.getMethod();

    _internalUri = _request.getUri();
    if (location) {
        if (location->getReturn()) {
            _response.setStatusCode(HTTP_MOVED_PERMANENTLY);
            _response.setLocation("http://localhost:8080" +
                                  urlDecode(*location->getReturn()));
            _response.setContent(GetHtmlErrorPage(_response));
            _response.setContentType("text/html");
            return;
        } else if (location->getAlias()) {
            _internalUri =
                *location->getAlias() +
                _request.getUri().substr((location->getName()).length());
        } else if (*location->getRoot() != "./")
            _rootPath = *location->getRoot();
        _autoindex = location->getAutoIndex();
    } else
        _rootPath = _serverConfig.getRoot();

    if (method == "GET") {
        if (!location || (location && location->getMethodIsAllowed("GET")))
            processGetRequest();
        else {
            _response.setStatusCode(HTTP_METHOD_NOT_ALLOWED);
        }
    } else if (method == "POST") {
        if (!location || (location && location->getMethodIsAllowed("POST")))
            processPostRequest();
        else
            _response.setStatusCode(HTTP_METHOD_NOT_ALLOWED);
    } else if (method == "DELETE") {
        if (!location || (location && location->getMethodIsAllowed("DELETE")))
            processDeleteRequest();
        else
            _response.setStatusCode(HTTP_METHOD_NOT_ALLOWED);
    } else
        _response.setStatusCode(HTTP_NOT_IMPLEMENTED);
}

void RequestHandler::processGetRequest() {
    std::string     fullPath;
    const Location* location = _serverConfig.getLocation(_internalUri);

    fullPath = _rootPath + _internalUri;

    if (isDirectory(fullPath)) {
        if (location && location->getIndex() &&
            isReadable(fullPath + "/" + *location->getIndex())) {
            fullPath += "/" + *location->getIndex();
        } else if (_serverConfig.getIndex() &&
                   isReadable(fullPath + "/" + *_serverConfig.getIndex())) {
            fullPath += "/" + *_serverConfig.getIndex();
        }
    }
    if (!pathExist(fullPath)) {
        _response.setStatusCode(HTTP_NOT_FOUND);
        return;
    }
    if (isDirectory(fullPath)) {
        if (_autoindex) {
            _response.setStatusCode(HTTP_OK);
            _response.setContent(getHtmlIndexPage(_rootPath, _internalUri));
            _response.setContentType("text/html");
            return;
        } else {
            _response.setStatusCode(HTTP_FORBIDDEN);
            return;
        }
    }
    if (!isReadable(fullPath) || !isFile(fullPath)) {
        _response.setStatusCode(HTTP_FORBIDDEN);
        return;
    }
    std::ifstream file(fullPath.c_str());
    if (file.fail()) {
        _response.setStatusCode(HTTP_FORBIDDEN);
        return;
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    _response.setContent(ss.str());
    _response.setContentType(MimeTypes::getType(fullPath.c_str()));
    _response.setLastModified(getLastModifiedTime(fullPath));

    file.close();
}

void RequestHandler::processPostRequest() {
    std::ofstream file;
    std::string   fullPath = _rootPath + _internalUri;

    if (_request.getContentType().find("multipart/form-data") !=
        std::string::npos) {
        if (_serverConfig.getUplaodDir().empty()) {
            _response.setStatusCode(HTTP_FORBIDDEN);
            return;
        }

        std::string boundary = extractBoundary(_request.getContentType());
        if (boundary.empty()) {
            _response.setStatusCode(HTTP_BAD_REQUEST);
            _response.setContent("Missing boundary in multipart data");
            return;
        }

        Logger::debug() << "upload dir: " << _serverConfig.getUplaodDir();
        MultipartParser parser(boundary, _serverConfig.getUplaodDir());
        std::string     chunk;
        while (_request.readBodyChunk(chunk)) {
            if (!parser.parseChunk(chunk)) {
                _response.setStatusCode(HTTP_INTERNAL_SERVER_ERROR);
                _response.setContent("Failed to parse multipart data");
                return;
            }
        }
        if (parser.hasError()) {
            _response.setStatusCode(HTTP_BAD_REQUEST);
            _response.setContent("Multipart parsing error: " +
                                 parser.getErrorMessage());
            return;
        }
        if (!parser.isComplete()) {
            _response.setStatusCode(HTTP_BAD_REQUEST);
            _response.setContent("Incomplete multipart data");
            return;
        }
        _response.setStatusCode(HTTP_CREATED);
        return;
    }
     _response.setStatusCode(HTTP_UNSUPPORTED_MEDIA_TYPE);
}

void RequestHandler::processDeleteRequest() {
    std::string fullPath = _rootPath + _request.getUri();

    if (!pathExist(fullPath)) {
        _response.setStatusCode(HTTP_NOT_FOUND);
        return;
    }
    if (remove(fullPath.c_str()) != 0) {
        _response.setStatusCode(HTTP_INTERNAL_SERVER_ERROR);
        return;
    }
    _response.setStatusCode(HTTP_NO_CONTENT);
}

void RequestHandler::generateErrorResponse(StatusCode         status_code,
                                           const std::string& error_msg) {
    (void)error_msg;  // use this to pass the error message to the response
    _response.setStatusCode(status_code);
    _response.setContent(GetHtmlErrorPage(_response));
    _response.setContentType("text/html");
    _response.setConnection("close");
}

std::string RequestHandler::extractBoundary(const std::string& content_type) {
    // Chercher "boundary=" dans le Content-Type
    size_t boundary_pos = content_type.find("boundary=");
    if (boundary_pos == std::string::npos) {
        return "";
    }

    boundary_pos += 9;  // Longueur de "boundary="

    // La boundary peut être entre guillemets ou non
    std::string boundary;
    if (boundary_pos < content_type.size() &&
        content_type[boundary_pos] == '"') {
        // Boundary entre guillemets
        boundary_pos++;
        size_t end_pos = content_type.find('"', boundary_pos);
        if (end_pos != std::string::npos) {
            boundary =
                content_type.substr(boundary_pos, end_pos - boundary_pos);
        }
    } else {
        // Boundary sans guillemets (jusqu'au prochain ;, espace ou fin)
        size_t end_pos = content_type.find_first_of("; \t\r\n", boundary_pos);
        if (end_pos == std::string::npos) {
            end_pos = content_type.size();
        }
        boundary = content_type.substr(boundary_pos, end_pos - boundary_pos);
    }

    return boundary;
}
