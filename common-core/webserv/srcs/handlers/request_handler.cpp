#include "request_handler.hpp"

#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "../core/server_status.hpp"
#include "../handlers/cgi_handler.hpp"
#include "../http/http_request.hpp"
#include "../http/http_response.hpp"
#include "../http/mime_types.hpp"
#include "../http/multipart_parser.hpp"
#include "../http/utils.hpp"
#include "../parsing/GlobalConfig.hpp"
#include "Logger.hpp"
#include "http_utils.hpp"
#include "lib/file_utils.hpp"
#include "lib/utils.hpp"

RequestHandler::RequestHandler(const HttpRequest&  request,
                               const ServerConfig& serverConfig)
    : _serverConfig(serverConfig),
      _request(request),
      _cgiHandler(NULL),
      _response(serverConfig),
      _rootPath(serverConfig.getRoot()),
      _internalUri(""),
      _autoindex(serverConfig.getAutoIndex()),
      _isStaticFile(false),
      _staticFilePath(""),
      _uploadInProgress(false),
      _uploadDone(false),
      _uploadOk(false),
      _uploadErrMsg(),
      _uploader(NULL) {};

RequestHandler::~RequestHandler() {
    if (_cgiHandler) {
        _cgiHandler->cleanupAsyncCgi();
        delete _cgiHandler;
        _cgiHandler = NULL;
    }
    if (_uploader) {
        delete _uploader;
        _uploader = NULL;
    }
}

const std::string RequestHandler::getRootPath() const {
    return _rootPath;
}

bool RequestHandler::shouldCloseConnection() const {
    if (_response.getStatusCode() != HTTP_OK) {
        return true;
    }
    return !_request.shouldKeepAlive();
}

void RequestHandler::handleRequest() {
    ServerStatus::getInstance().onRequestStarted();

    Logger::info() << "Request: " << _request.getMethod() << " "
                   << _request.getUri() << " " << _request.getVersion();

    processRequest();

    if (_request.shouldKeepAlive()) {
        _response.setConnection("keep-alive");
    } else {
        _response.setConnection("close");
    }
    if (_response.getStatusCode() != HTTP_OK &&
        _response.getStatusCode() != HTTP_MOVED_PERMANENTLY &&
        _response.getStatusCode() != HTTP_NO_CONTENT) {
        _response.CreateErrorPage();
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
        _response.CreateErrorPage();
        std::string redirect = *location->getReturn();
        std::string port = lib ::to_string(_serverConfig.getPort());
        _response.setLocation("http://" + _serverConfig.getHost() + ":" + port +
                              urlDecode(redirect));
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
        if (!_cgiHandler) {
            _response.setStatusCode(HTTP_INTERNAL_SERVER_ERROR);
        }
        return;
    }

    if (lib::isDirectory(fullPath)) {
        resolveIndexFile(fullPath, location, _serverConfig.getIndex());
    }

    if (_internalUri == "/status") {
        handleStatusRequest();
        return;
    } else if (_internalUri == "/config") {
        _response.setStatusCode(HTTP_OK);
        _response.setContent(JsonifyServerConfig());
        _response.setContentType("application/json");
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
        _response.setContentLength(static_cast<size_t>(st.st_size));
    }

    _response.setContentType(MimeTypes::getType(fullPath.c_str()));
    _response.setLastModified(lib::getLastModifiedTime(fullPath));
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

        if (!startUpload(boundary)) {
            _response.setStatusCode(HTTP_INTERNAL_SERVER_ERROR);
            _response.setContent("Failed to initialize upload parsing");
        }

        return;
    }
    _response.setStatusCode(HTTP_UNSUPPORTED_MEDIA_TYPE);
}

// ============ DELETE ============ //

void RequestHandler::processDeleteRequest() {
    std::string fullPath = _rootPath + _request.getUri();

    if (!lib::pathExist(fullPath)) {
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
		_response.setContent("");
		_response.CreateErrorPage(HTTP_GATEWAY_TIMEOUT);
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
		_response.setContent("");
		_response.CreateErrorPage(HTTP_GATEWAY_TIMEOUT);
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

bool RequestHandler::startUpload(const std::string& boundary) {
    if (_uploadInProgress || _uploader) {
        return false;
    }
    _uploader = new MultipartParser(boundary, _serverConfig.getUploadDir());
    if (_uploader == NULL) {
        return false;
    }
    _request.resetBodyReader();
    _uploadInProgress = true;
    _uploadDone = false;
    _uploadOk = false;
    _uploadErrMsg.clear();
    return true;
}

// ============ UPLOAD PROCESSING ============ //

bool RequestHandler::hasUploadInProgress() const {
    return _uploadInProgress && !_uploadDone;
}

bool RequestHandler::processUploadChunk() {
    if (!_uploadInProgress || _uploadDone || _uploader == NULL) {
        return _uploadDone;
    }

    std::string chunk;

    // 256kb per chunk, to adjust if needed
    const size_t step_budget = 256 * 1024;
    if (!_request.readBodyChunk(chunk, step_budget)) {
        if (_request.hasMoreBody()) {
            _uploadErrMsg = "Failed to read request body";
            _uploadOk = false;
            _uploadDone = true;
            _uploadInProgress = false;
            return true;
        }

        if (_uploader->hasError()) {
            _uploadErrMsg = _uploader->getErrorMessage();
            _uploadOk = false;
        } else if (!_uploader->isComplete()) {
            _uploadErrMsg = "Incomplete multipart data";
            _uploadOk = false;
        } else {
            _uploadOk = true;
        }
        _uploadDone = true;
        _uploadInProgress = false;
        return true;
    }

    if (!_uploader->parseChunk(chunk)) {
        _uploadErrMsg = _uploader->getErrorMessage();
        _uploadDone = true;
        _uploadOk = false;
        _uploadInProgress = false;
        return true;
    }

    if (!_request.hasMoreBody()) {
        if (_uploader->hasError()) {
            _uploadErrMsg = _uploader->getErrorMessage();
            _uploadOk = false;
        } else if (!_uploader->isComplete()) {
            _uploadErrMsg = "Incomplete multipart data";
            _uploadOk = false;
        } else {
            _uploadOk = true;
        }
        _uploadDone = true;
        _uploadInProgress = false;
        return true;
    }

    return false;
}

bool RequestHandler::uploadSucceeded() const {
    return _uploadDone && _uploadOk;
}

const std::string& RequestHandler::uploadErrorMessage() const {
    return _uploadErrMsg;
}

std::string RequestHandler::JsonifyServerConfig() {
    std::stringstream json;

    json << "{" << std::endl;
    json << "  \"server_name\": \"" << _serverConfig.getServerName() << "\","
         << std::endl;
    json << "  \"port\": " << _serverConfig.getPort() << "," << std::endl;
    json << "  \"root\": \"" << _serverConfig.getRoot() << "\"," << std::endl;
    const char* index = _serverConfig.getIndex()->c_str();
    json << "  \"index\": \"" << (index ? index : "") << "\"," << std::endl;
    json << "  \"autoindex\": "
         << (_serverConfig.getAutoIndex() ? "\"on\"" : "\"off\"") << ","
         << std::endl;
    json << "  \"upload_dir\": \"" << _serverConfig.getUploadDir() << "\","
         << std::endl;
    json << "  \"client_max_body_size\": \""
         << lib::to_string(_serverConfig.getClientMaxBodySize()) << "\","
         << std::endl;
    json << "  \"locations\": [" << std::endl;
    for (std::map<std::string, Location>::const_iterator it =
             _serverConfig.route.begin();
         it != _serverConfig.route.end(); ++it) {
        const Location& location = it->second;
        json << "    {" << std::endl;
        json << "      \"path\": \"" << location.getName() << "\","
             << std::endl;
        json << "      \"root\": \""
             << (location.getRoot() ? *location.getRoot() : "") << "\","
             << std::endl;
        json << "      \"methods\": [" << std::endl;

        bool firstMethod = true;
        if (location.getMethodIsAllowed("GET")) {
            if (!firstMethod)
                json << "," << std::endl;
            json << "        \"GET\"";
            firstMethod = false;
        }
        if (location.getMethodIsAllowed("POST")) {
            if (!firstMethod)
                json << "," << std::endl;
            json << "        \"POST\"";
            firstMethod = false;
        }
        if (location.getMethodIsAllowed("DELETE")) {
            if (!firstMethod)
                json << "," << std::endl;
            json << "        \"DELETE\"";
            firstMethod = false;
        }

        json << std::endl << "      ]," << std::endl;
        json << "      \"autoindex\": "
             << (location.getAutoIndex() ? "\"on\"" : "\"off\"") << ","
             << std::endl;
        const std::string* index = location.getIndex();
        json << "      \"index\": \"" << (index ? *index : "") << "\","
             << std::endl;
        const std::string* return_value = location.getReturn();
        json << "      \"return\": \"" << (return_value ? *return_value : "")
             << "\"," << std::endl;
        const std::string* alias = location.getAlias();
        json << "      \"client_max_body_size\": \""
             << lib::to_string(location.getClientMaxBodySize()) << "\","
             << std::endl;
        json << "      \"alias\": \"" << (alias ? *alias : "") << "\""
             << std::endl;
        json << "    }";

        // Add comma if this isn't the last location
        std::map<std::string, Location>::const_iterator it_temp = it;
        ++it_temp;
        if (it_temp != _serverConfig.route.end()) {
            json << ",";
        }
        json << std::endl;
    }

    json << "  ]" << std::endl;
    json << "}" << std::endl;

    return json.str();
}
