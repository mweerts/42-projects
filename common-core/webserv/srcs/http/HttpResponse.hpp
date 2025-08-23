#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <map>
#include <string>

#include "http_status_code.hpp"

class HttpResponse {
   public:
    HttpResponse();
    ~HttpResponse();

    void        setStatusCode(StatusCode statusCode);
    void        setContent(const std::string& content);
    void        setContentType(const std::string& contentType);
    void        setConnection(const std::string& connection);
    void        setLastModified(const std::string& lastModified);
    void        setLocation(const std::string& location);
    void        setContentLength(int length);
	void        setHeader(const std::string& key, const std::string& value);

	void        removeAdditionnalHeader(const std::string& key);
	const std::map<std::string, std::string>& getAdditionnalHeaders() const;
	std::string getAdditionnalHeader(const std::string& key) const;
	
    StatusCode  getStatusCode() const;
    std::string getServerName() const;
    std::string getConnection() const;
    const std::string& getContent() const { return _content; }
    int         getContentLength() const { return _contentLength; }

    std::string toString();
    std::string headersToString();

   private:
    StatusCode  _statusCode;
    std::string _version;
    std::string _serverName;
    std::string _date;
    std::string _contentType;
    int         _contentLength;
    std::string _lastModified;
    std::string _connection;
    std::string _content;
    std::string _location;
	std::map<std::string, std::string> _additionnalHeaders;

    void setDate(void);
};

#endif
