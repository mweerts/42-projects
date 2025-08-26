### Core Server Statistics
- [ ] **Average Response Time** - Mean response time in milliseconds

---

### Configuration Details
- **Client Max Body Size** - Upload size limit
  - Source: `ServerConfig::getClientMaxBodySize()`
  - Effort: Low (expose existing getter)
  
- **Upload Directory** - File upload location
  - Source: `ServerConfig::getUploadDir()`
  - Effort: Low (expose existing getter)

### Server Features Status
- **Autoindex Status** - Directory listing enabled/disabled
  - Source: `ServerConfig::getAutoIndex()`
  - Effort: Low (expose existing getter)
  
- **Supported HTTP Methods** - GET, POST, DELETE availability
  - Source: Location config allow_methods
  - Effort: Medium (parse and expose method configs)
  
- **CGI Bin Status** - CGI binary availability
  - Source: `ServerConfig::getCgiBin()`
  - Effort: Low (expose existing getter)


## 📋 Implementation Checklist
- [ ] Update status page HTML to display new metrics

