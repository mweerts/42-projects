#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <map>
#include <string>

#include "http_utils.hpp"

class HttpResponse {
   public:
    HttpResponse();
    HttpResponse(std::map<StatusCode, std::string> error_pages);
    ~HttpResponse();

    void setStatusCode(StatusCode statusCode);
    void setContent(const std::string& content);
    void setContentType(const std::string& contentType);
    void setConnection(const std::string& connection);
    void setLastModified(const std::string& lastModified);
    void setLocation(const std::string& location);
    void setContentLength(size_t length);
    void setHeader(const std::string& key, const std::string& value);
    void setErrorPagePath(const std::string& error_page_path);

    void removeAdditionnalHeader(const std::string& key);
    const std::map<std::string, std::string>& getAdditionnalHeaders() const;
    std::string getAdditionnalHeader(const std::string& key) const;

    StatusCode         getStatusCode() const;
    std::string        getServerName() const;
    std::string        getConnection() const;
    const std::string& getContent() const;
    size_t             getContentLength() const;

    std::string toString();
    std::string headersToString();
    void        CreateErrorPage(const std::string& extra_info = "");

   private:
    StatusCode  _statusCode;
    std::string _version;
    std::string _serverName;
    std::string _date;
    std::string _contentType;
    size_t      _contentLength;
    std::string _lastModified;
    std::string _connection;
    std::string _content;
    std::string _location;

    std::map<StatusCode, std::string>  _error_pages;
    std::map<std::string, std::string> _additionnalHeaders;

    void setDate(void);
};

#endif
