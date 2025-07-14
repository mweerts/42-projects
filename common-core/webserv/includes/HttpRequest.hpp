#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <map>
#include <string>

class HttpRequest {
   public:
    HttpRequest();
    ~HttpRequest();

    void setMethod(const std::string& method);
    void setUri(const std::string& uri);
    void setVersion(const std::string& version);
    void setHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body);
    const std::string&                        getMethod() const;
    const std::string&                        getUri() const;
    const std::string&                        getVersion() const;
    const std::map<std::string, std::string>& getHeaders() const;
    const std::string&                        getBody() const;

   private:
    std::string                        _method;
    std::string                        _uri;
    std::string                        _version;
    std::map<std::string, std::string> _headers;
    std::string                        _body;
};

#endif