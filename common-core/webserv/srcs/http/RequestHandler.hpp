#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "../srcs/parsing/GlobalConfig.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "http_status_code.hpp"

class ServerConfig;
class CgiHandler;

class RequestHandler {
   public:
    RequestHandler(const HttpRequest&  request,
                   const ServerConfig& serverConfig);
    ~RequestHandler();

    void handleRequest();
    void sendResponse(int socket_fd);
    bool shouldCloseConnection() const;

    HttpResponse& getResponse();

    void generateErrorResponse(StatusCode         error_code,
                               const std::string& error_msg);

    const std::string getRootPath() const;

    bool hasCgiRunning() const;
    bool processCgi();

   private:
    const ServerConfig& _serverConfig;
    const HttpRequest&  _request;
    CgiHandler*         _cgiHandler;
    HttpResponse        _response;
    std::string         _rootPath;
    std::string         _internalUri;
    bool                _autoindex;

    void setResponse(const HttpResponse& response);
    void processRequest();
    void processGetRequest();
    void processPostRequest();
    void processDeleteRequest();

};

#endif
