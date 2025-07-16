#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP



#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "server_config.hpp"

class RequestHandler {
   public:
    RequestHandler();
    ~RequestHandler();

    void handleRequest(const std::string& request);
    void setServerConfig(const ServerConfig& config);
    void sendResponse(int socket_fd);

   private:
    ServerConfig      _serverConfig;
    HttpRequest       _request;
    HttpResponse      _response;
    const std::string _rootPath;

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