#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <map>
#include <string>

#include "http_status_code.hpp"

class HttpResponse {
   public:
    HttpResponse();
    ~HttpResponse();

    void setStatusCode(StatusCode statusCode);
    void setHeader(const std::string& key, const std::string& value);
    void setContent(const std::string& content);
    void setContentType(const std::string& contentType);
    void setConnection(const std::string& connection);

    StatusCode  getStatusCode() const;
    std::string getServerName() const;
    std::string getConnection() const;

    std::string toString();

   private:
    StatusCode                         _statusCode;
    std::string                        _version;
    std::string                        _serverName;
    std::string                        _date;
    std::string                        _contentType;
    int                                _contentLength;
    std::string                        _connection;
    std::map<std::string, std::string> _headers;
    std::string                        _content;

    void setDate(void);
};

#endif