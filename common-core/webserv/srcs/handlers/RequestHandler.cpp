#include "RequestHandler.hpp"

#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>

#include "../core/server_status.hpp"
#include "../handlers/cgi_handler.hpp"
#include "../http/HttpRequest.hpp"
#include "../http/HttpResponse.hpp"
#include "../http/MimeTypes.hpp"
#include "../http/MultipartParser.hpp"
#include "../http/utils.hpp"
#include "../parsing/GlobalConfig.hpp"
#include "Logger.hpp"
#include "http_status_code.hpp"
#include "lib/file_utils.hpp"
#include "lib/utils.hpp"
#include "Logger.hpp"

RequestHandler::RequestHandler(const HttpRequest&  request,
                               const ServerConfig& serverConfig)
    : _serverConfig(serverConfig),
      _request(request),
      _cgiHandler(NULL),
      _rootPath(serverConfig.getRoot()),
      _autoindex(serverConfig.getAutoIndex()),
      _isStaticFile(false),
      _staticFilePath("") {};

RequestHandler::~RequestHandler() {
    if (_cgiHandler) {
        _cgiHandler->cleanupAsyncCgi();
        delete _cgiHandler;
        _cgiHandler = NULL;
    }
}

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
    ServerStatus::getInstance().onRequestProcessed();
}

// TODO: maybe make this a bit cleaner or easy to read?
void RequestHandler::processRequest() {
    _internalUri = lib::extractPathFromUri(_request.getUri());
    const Location*    location = _serverConfig.getLocation(_internalUri);
    const std::string& method = _request.getMethod();

    if (method == "GET" || method == "POST") {
        _queryString = lib::extractQueryFromUri(_request.getUri());
    }

    Logger::debug() << "processing request for uri: " << _internalUri;
    if (!_queryString.empty())
        Logger::debug() << "query string: " << _queryString;

    if (location) {
        if (location->getReturn()) {
            _response.setStatusCode(HTTP_MOVED_PERMANENTLY);
            // TODO: Needs to be dynamic
            _response.setLocation("http://localhost:8080" +
                                  urlDecode(*location->getReturn()));
            _response.setContent(GetHtmlErrorPage(_response));
            _response.setContentType("text/html");
            return;
        } else if (location->getAlias()) {
            _internalUri = *location->getAlias() +
                           _internalUri.substr((location->getName()).length());
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
        if (!location) {
            Logger::warning() << "No location found for POST request";
        }
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

// ============ GET ============ //

void RequestHandler::handleStatusRequest() {
    _response.setStatusCode(HTTP_OK);
    _response.setContent(ServerStatus::getInstance().getJson());
    _response.setContentType("application/json");
    // _response.setConnection("close");
}

void RequestHandler::processGetRequest() {
    std::string     fullPath;
    const Location* location = _serverConfig.getLocation(_internalUri);

    fullPath = _rootPath + _internalUri;
    CgiHandler tmpCgi(_request, &_serverConfig);
    if (tmpCgi.isCgiScript(_internalUri)) {
        _cgiHandler = initCgiHandler();
        if (!_cgiHandler)
            _response.setStatusCode(HTTP_INTERNAL_SERVER_ERROR);
        return;
    }

    if (lib::isDirectory(fullPath)) {
        if (location && location->getIndex() &&
            lib::isReadable(fullPath + "/" + *location->getIndex())) {
            fullPath += "/" + *location->getIndex();
        } else if (_serverConfig.getIndex() &&
                   lib::isReadable(fullPath + "/" +
                                   *_serverConfig.getIndex())) {
            fullPath += "/" + *_serverConfig.getIndex();
        }
    }

    if (_internalUri == "/status") {
        handleStatusRequest();
        return;
    }

    if (!pathExist(fullPath)) {
        _response.setStatusCode(HTTP_NOT_FOUND);
        return;
    }

    if (lib::isDirectory(fullPath)) {
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
    if (!lib::isReadable(fullPath) || !lib::isFile(fullPath)) {
        _response.setStatusCode(HTTP_FORBIDDEN);
        return;
    }

    // needed for the response streamer
    _isStaticFile = true;
    _staticFilePath = fullPath;
    struct stat st;
    if (stat(fullPath.c_str(), &st) == 0) {
        _response.setContentLength(static_cast<int>(st.st_size));
    }

    _response.setContentType(MimeTypes::getType(fullPath.c_str()));
    _response.setLastModified(getLastModifiedTime(fullPath));
}

// ============ POST ============ //

void RequestHandler::processPostRequest() {
    CgiHandler tmpCgi(_request, &_serverConfig);
    if (tmpCgi.isCgiScript(_internalUri)) {
        _cgiHandler = initCgiHandler();
        if (!_cgiHandler)
            _response.setStatusCode(HTTP_INTERNAL_SERVER_ERROR);
        return;
    }

    // Regular file upload handling
    std::ofstream file;
    std::string   fullPath = _rootPath + _internalUri;

    if (_request.getContentType().find("multipart/form-data") !=
        std::string::npos) {
        if (_serverConfig.getUploadDir().empty()) {
            _response.setStatusCode(HTTP_FORBIDDEN);
            return;
        }

        std::string boundary = extractBoundary(_request.getContentType());
        if (boundary.empty()) {
            _response.setStatusCode(HTTP_BAD_REQUEST);
            _response.setContent("Missing boundary in multipart data");
            return;
        }

        Logger::debug() << "upload dir: " << _serverConfig.getUploadDir();
        MultipartParser parser(boundary, _serverConfig.getUploadDir());
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

// ============ DELETE ============ //

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

// ============ CGI ============ //

CgiHandler* RequestHandler::initCgiHandler() {
    CgiHandler* cgiHandler = new CgiHandler(_request, &_serverConfig);
    cgiHandler->setQueryString(_queryString);
    if (cgiHandler->startAsyncCgi(_internalUri))
        return cgiHandler;
    delete cgiHandler;
    return NULL;
}

bool RequestHandler::hasCgiRunning() const {
    return _cgiHandler != NULL && _cgiHandler->isProcessing();
}

bool RequestHandler::processCgi() {
    if (!_cgiHandler || !_cgiHandler->isProcessing()) {
        return false;
    }

    if (_cgiHandler->isTimedOut()) {
        Logger::warning() << "CGI process timed out";
        _response.setStatusCode(HTTP_GATEWAY_TIMEOUT);
        _cgiHandler->cleanupAsyncCgi();
        delete _cgiHandler;
        _cgiHandler = NULL;
        return true;
    }

    if (_cgiHandler->processCgiIO()) {
        _cgiHandler->buildCgiResponse(_response);
        _cgiHandler->cleanupAsyncCgi();
        delete _cgiHandler;
        _cgiHandler = NULL;
        return true;
    }

    return false;
}

bool RequestHandler::handleCgiFdEvent(int fd, short revents) {
    if (!_cgiHandler)
        return true;

	if (_cgiHandler->isTimedOut()) {
		Logger::warning() << "CGI process timed out";
		_response.setStatusCode(HTTP_GATEWAY_TIMEOUT);
		_cgiHandler->cleanupAsyncCgi();
		delete _cgiHandler;
		_cgiHandler = NULL;
		return true;
	}

    bool done = _cgiHandler->handleFdEvent(fd, revents);
    if (done) {
        _cgiHandler->buildCgiResponse(_response);
        _cgiHandler->cleanupAsyncCgi();
        delete _cgiHandler;
        _cgiHandler = NULL;
        return true;
    }
    return false;
}

int RequestHandler::getCgiInputPipe() const {
    if (_cgiHandler)
        return _cgiHandler->getInputPipe();
    return -1;
}

int RequestHandler::getCgiOutputPipe() const {
    if (_cgiHandler)
        return _cgiHandler->getOutputPipe();
    return -1;
}

// ============ UTILS ============ //

bool RequestHandler::isStaticFileResponse() const {
    return _isStaticFile;
}

const std::string& RequestHandler::getStaticFilePath() const {
    return _staticFilePath;
}

HttpResponse& RequestHandler::getResponse() {
    return _response;
}

void RequestHandler::generateErrorResponse(StatusCode         status_code,
                                           const std::string& error_msg) {
    (void)error_msg;  // not implemented
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
