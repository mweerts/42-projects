#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "../http/HttpRequest.hpp"
#include "../http/HttpResponse.hpp"
#include "../srcs/parsing/GlobalConfig.hpp"
#include "http_status_code.hpp"

class ServerConfig;
class CgiHandler;
class MultipartParser;

class RequestHandler {
   public:
    RequestHandler(const HttpRequest&  request,
                   const ServerConfig& serverConfig);
    ~RequestHandler();

    void handleRequest();
    bool shouldCloseConnection() const;

    HttpResponse& getResponse();

    void generateErrorResponse(StatusCode         error_code,
                               const std::string& error_msg);

    const std::string getRootPath() const;

    bool hasCgiRunning() const;
    bool processCgi();
    int  getCgiInputPipe() const;
    int  getCgiOutputPipe() const;
    bool handleCgiFdEvent(int fd, short revents);

    bool               isStaticFileResponse() const;
    const std::string& getStaticFilePath() const;

    bool               hasUploadInProgress() const;
    bool               uploadSucceeded() const;
    bool               processUploadChunk();
    const std::string& uploadErrorMessage() const;

   private:
    const ServerConfig& _serverConfig;
    const HttpRequest&  _request;
    CgiHandler*         _cgiHandler;
    HttpResponse        _response;
    std::string         _rootPath;
    std::string         _queryString;
    std::string         _internalUri;
    bool                _autoindex;
    bool                _isStaticFile;
    std::string         _staticFilePath;

    bool             _uploadInProgress;
    bool             _uploadDone;
    bool             _uploadOk;
    std::string      _uploadErrMsg;
    MultipartParser* _uploader;

    void processRequest();
    void processGetRequest();
    void processPostRequest();
    void processDeleteRequest();
    void handleStatusRequest();
    bool startUpload(const std::string& boundary);

    CgiHandler* initCgiHandler();

    std::string extractBoundary(const std::string& content_type);
    std::string JsonifyServerConfig();
    void        handleRedirect();
};

#endif
