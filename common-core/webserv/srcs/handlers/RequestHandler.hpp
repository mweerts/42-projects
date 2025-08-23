#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "../http/HttpRequest.hpp"
#include "../http/HttpResponse.hpp"
#include "../srcs/parsing/GlobalConfig.hpp"
#include "http_status_code.hpp"

class ServerConfig;
class CgiHandler;

enum RequestMethod {
    GET,
    POST,
    DELETE,
	OPTIONS,
	HEAD,
	CONNECT,
	TRACE,
	PATCH,
	PUT,
	PROPFIND,
	PROPPATCH,
	UNKNOWN,
};

class RequestHandler {
   public:
    RequestHandler(const HttpRequest&  request,
                   const ServerConfig& serverConfig);
    ~RequestHandler();

    void handleRequest();
    // void sendResponse(int socket_fd);
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

    bool isStaticFileResponse() const;
    const std::string& getStaticFilePath() const;

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

    void setResponse(const HttpResponse& response);
    void processRequest();
    void processGetRequest();
    void processPostRequest();
    void processDeleteRequest();
	void handleStatusRequest();

    CgiHandler* initCgiHandler();

    std::string extractBoundary(const std::string& content_type);

	RequestMethod getRequestMethod(const std::string& method);
	void handleRedirect();
};

#endif
