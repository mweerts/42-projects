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

RequestHandler::RequestHandler(const HttpRequest&  request,
                               const ServerConfig& serverConfig)
    : _serverConfig(serverConfig),
      _request(request),
      _cgiHandler(NULL),
      _rootPath(serverConfig.getRoot()),
      _internalUri(""),
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

static void replaceErrorPlaceholders(std::string&        content,
                                     const HttpResponse& response) {
    std::string errCode = lib::to_string(response.getStatusCode());

    size_t pos = 0;
    while ((pos = content.find("{{ERROR_CODE}}")) != std::string::npos) {
        content.replace(pos, 14, errCode);
    }

    while ((pos = content.find("{{ERROR_MESSAGE}}")) != std::string::npos) {
        std::string errMessage = GetHttpStatusText(response.getStatusCode());
        content.replace(pos, 17, errMessage);
    }
}

void RequestHandler::handleRequest() {
    ServerStatus::getInstance().onRequestStarted();
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
        if (errorPage) {
            std::ifstream file((_rootPath + "/" + *errorPage).c_str());
            if (file.fail()) {
                _response.setContent(GetHtmlErrorPage(_response));
                _response.setContentType("text/html");
                return;
            }
            std::ostringstream ss;
            ss << file.rdbuf();
            std::string content = ss.str();

            replaceErrorPlaceholders(content, _response);
            _response.setContent(content);
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

// Handles redirects, aliases and root paths
void RequestHandler::handleRedirect() {
    const Location* location = _serverConfig.getLocation(_internalUri);

    if (!location) {
        _rootPath = _serverConfig.getRoot();
        return;
    }
    if (location->getReturn()) {
        _response.setStatusCode(HTTP_MOVED_PERMANENTLY);
        std::string redirect = *location->getReturn();
        std::string port = lib ::to_string(_serverConfig.getPort());
        _response.setLocation("http://" + _serverConfig.getHost() + ":" + port +
                              urlDecode(redirect));
        _response.setContent(GetHtmlErrorPage(_response));  // 301
        _response.setContentType("text/html");
        Logger::info() << "redirecting to: " << urlDecode(redirect);
        return;
    }
    if (location->getAlias()) {
        
        std::string alias = *location->getAlias();
        std::string name = location->getName();
        _internalUri = alias + _internalUri.substr(name.length());
        Logger::debug() << "aliasing to: " << _internalUri;
        return;
    }
    if (location->getRoot()) {
        _rootPath = *location->getRoot();
        return;
    }
}

void RequestHandler::processRequest() {
    _internalUri = lib::extractPathFromUri(_request.getUri());
    const std::string& method = _request.getMethod();

    Logger::debug() << "processing request for uri: " << _internalUri
                    << (!_queryString.empty() ? " query: " + _queryString : "");

    if (method == "GET" || method == "POST") {
        _queryString = lib::extractQueryFromUri(_request.getUri());
    }

    handleRedirect();

    if (_response.getStatusCode() == HTTP_MOVED_PERMANENTLY) {
        return;
    }

    RequestMethod requestMethod = getRequestMethod(method);

    const Location* location = _serverConfig.getLocation(_internalUri);
    if (!location && method == "DELETE") {
        _response.setStatusCode(HTTP_METHOD_NOT_ALLOWED);
        return;
    }
    if (location) {
        if (!location->getMethodIsAllowed(method)) {
            _response.setStatusCode(HTTP_METHOD_NOT_ALLOWED);
            return;
        }
        _autoindex = location->getAutoIndex();
    }

    switch (requestMethod) {
        case GET: processGetRequest(); break;
        case POST: processPostRequest(); break;
        case DELETE: processDeleteRequest(); break;
        case PUT:
        case PATCH:
        case HEAD:
        case OPTIONS:
        case CONNECT:
        case TRACE:
        case PROPFIND:
        case PROPPATCH: _response.setStatusCode(HTTP_NOT_IMPLEMENTED); break;
        case UNKNOWN: _response.setStatusCode(HTTP_BAD_REQUEST); break;
    }
}

// ============ GET ============ //

void RequestHandler::handleStatusRequest() {
    _response.setStatusCode(HTTP_OK);
    _response.setContent(ServerStatus::getInstance().getJson());
    _response.setContentType("application/json");
    // _response.setConnection("close");
}

static void resolveIndexFile(std::string& fullPath, const Location* location,
                             const std::string* serverIndex) {
    const std::string* indexFile = NULL;

    if (location && location->getIndex()) {
        indexFile = location->getIndex();
    } else if (serverIndex) {
        indexFile = serverIndex;
    }

    if (indexFile && !indexFile->empty()) {
        if (lib::isReadable(fullPath + "/" + *indexFile)) {
            fullPath += "/" + *indexFile;
        }
    }
}

void RequestHandler::processGetRequest() {
    std::string     fullPath = "";
    const Location* location = _serverConfig.getLocation(_internalUri);

    fullPath = _rootPath + _internalUri;
    if (fullPath.empty() || _rootPath.empty() || _internalUri.empty()) {
        _response.setStatusCode(HTTP_BAD_REQUEST);
        return;
    }
    CgiHandler tmpCgi(_request, &_serverConfig);
    if (tmpCgi.isCgiScript(_internalUri)) {
        _cgiHandler = initCgiHandler();
        if (!_cgiHandler)
            _response.setStatusCode(HTTP_INTERNAL_SERVER_ERROR);
        return;
    }

    if (lib::isDirectory(fullPath)) {
        resolveIndexFile(fullPath, location, _serverConfig.getIndex());
    }

    if (_internalUri == "/status") {
        handleStatusRequest();
        return;
    }

    if (!lib::pathExist(fullPath)) {
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
    if (!_cgiHandler) {
        return true;
    }

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

RequestMethod RequestHandler::getRequestMethod(const std::string& method) {
    if (method.length() > 10)
        return UNKNOWN;

    switch (method[0]) {
        case 'G':
            if (method == "GET")
                return GET;
            break;
        case 'P':
            if (method == "POST")
                return POST;
            if (method == "PUT")
                return PUT;
            if (method == "PATCH")
                return PATCH;
            if (method == "PROPFIND")
                return PROPFIND;
            if (method == "PROPPATCH")
                return PROPPATCH;
            break;
        case 'D':
            if (method == "DELETE")
                return DELETE;
            break;
        case 'H':
            if (method == "HEAD")
                return HEAD;
            break;
        case 'O':
            if (method == "OPTIONS")
                return OPTIONS;
            break;
        case 'C':
            if (method == "CONNECT")
                return CONNECT;
            break;
        case 'T':
            if (method == "TRACE")
                return TRACE;
            break;
    }
    return UNKNOWN;
}
