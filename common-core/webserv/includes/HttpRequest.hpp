#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <fstream>
#include <map>
#include <string>
#include <vector>

class HttpRequest {
   public:
    HttpRequest();
    HttpRequest(const HttpRequest& other);
    HttpRequest& operator=(const HttpRequest& other);
    ~HttpRequest();

    void setMethod(const std::string& method);
    void setUri(const std::string& uri);
    void setVersion(const std::string& version);
    void setHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body);
    void setRequestFilepath(const std::string& filepath);
    void setBodyParams(const std::string& filepath, size_t start_pos,
                       size_t length);

    const std::string& getMethod() const;
    const std::string& getUri() const;
    const std::string& getVersion() const;
    size_t             getBodySize() const;
    const std::string& getRequestFilepath() const;

    // Headers utility getters
    const std::map<std::string, std::string>& getHeaders() const;
    std::string                               getContentType() const;
    size_t                                    getContentLength() const;
    bool                                      shouldKeepAlive() const;

    // Body reader methods
    bool readBodyChunk(std::string& chunk, size_t max_bytes = 1024) const;
    std::string readBodyAll() const;
    void        resetBodyReader() const;

    bool hasMoreBody() const;
    bool isRequestChunked() const;

    void reset();  // Reset the request for reuse by the same client

   private:
    std::string                        _method;
    std::string                        _uri;
    std::string                        _version;
    std::map<std::string, std::string> _headers;

    std::string _request_tmp_file;

    std::string _body;
    size_t      _body_start;
    size_t      _body_length;
    bool        _body_is_file;

    mutable std::ifstream _body_file_stream;
    mutable size_t        _body_read_pos;
    mutable bool          _body_reader_initialized;

    bool readBodyChunkFromMemory(std::string& chunk, size_t max_bytes) const;
    bool readBodyChunkFromFile(std::string& chunk, size_t max_bytes) const;
};

std::string   urlDecode(const std::string& encoded);
std::ostream& operator<<(std::ostream& os, const HttpRequest& request);

#endif
