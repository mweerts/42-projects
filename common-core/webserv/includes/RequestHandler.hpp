#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "../srcs/parsing/GlobalConfig.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class ServerConfig;

class RequestHandler {
   public:
    RequestHandler(const ServerConfig& serverConfig)
        : _serverConfig(serverConfig),
          _rootPath(serverConfig.getRoot()),
          _autoindex(serverConfig.getAutoIndex()) {
          };
    ~RequestHandler();

    void handleRequest(const std::string& request);
    void sendResponse(int socket_fd);

   private:
    const ServerConfig& _serverConfig;
    HttpRequest         _request;
    HttpResponse        _response;
    std::string         _rootPath;
    std::string         _internalUri;
    bool                _autoindex;

    void setRequest(const HttpRequest& request);
    void setResponse(const HttpResponse& response);
    void processRequest();
    void processGetRequest();
    void processPostRequest();
    void processDeleteRequest();
    void parseFullRequest(const std::string& request);
    void parseRequestLine(const std::string& requestLine);
    void parseHeaders(const std::string& headers);
    void parseBody(const std::string& body);

    const std::string getRootPath() const;
};

#endif
