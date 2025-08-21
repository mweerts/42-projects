### Core Server Statistics
- ✅ **Server Uptime** - Time since server started (formatted as "Xd Xh Xm Xs")
- ✅ **Active Connections** - Current number of active connections
- ✅ **Total Connections** - All-time connection count
- ✅ **Requests per Minute** - Current request rate (rolling 1-minute window)
- ✅ **Total Requests** - All-time request count
- ✅ **Average Response Time** - Mean response time in milliseconds

---

## 🔴 High Priority (Easy to Implement, High Value)

These features require minimal backend work and provide significant monitoring value:

### Network & Port Information
  
- **Server Hosts** - Server hostnames/IPs
  - Source: `ServerConfig::getHost()` for each server
  - Effort: Low (iterate through server configs)
  
- **Server Count** - Number of configured servers
  - Source: `GlobalConfig::getServers().size()`
  - Effort: Very Low (expose existing count)

### System Resources
- **Process ID (PID)** - WebServ process ID
  - Source: `getpid()` system call
  - Effort: Very Low (add to ServerStatus)
  
- **Memory Usage** - Current memory consumption
  - Source: `/proc/self/status` (Linux) or `getrusage()` (Unix)
  - Effort: Low (add memory tracking)
  
- **File Descriptors** - Open file descriptor count
  - Source: `/proc/self/fd` directory listing
  - Effort: Low (add FD counting)

---

### Configuration Details
- **Client Max Body Size** - Upload size limit
  - Source: `ServerConfig::getClientMaxBodySize()`
  - Effort: Low (expose existing getter)
  
- **Root Directory** - Server root path
  - Source: `ServerConfig::getRoot()`
  - Effort: Low (expose existing getter)
  
- **Upload Directory** - File upload location
  - Source: `ServerConfig::getUploadDir()`
  - Effort: Low (expose existing getter)

### File System Information
- **Disk Usage** - Available space in upload/root directories
  - Source: `statvfs()` system call
  - Effort: Medium (add disk space monitoring)

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

---

## 🟠 Low Priority (Complex to Implement, Nice to Have)

These features require significant backend work but provide advanced monitoring capabilities:

### Advanced Performance Metrics
- **Response Time Distribution** - Min/max/percentile response times
  - Source: Enhanced response time tracking
  - Effort: High (modify request processing)
  
- **Bandwidth Usage** - Bytes sent/received per minute
  - Source: Enhanced connection monitoring
  - Effort: High (add byte counting)
  
- **Connection States** - Established, closing, time-wait counts
  - Source: Enhanced connection state tracking
  - Effort: High (modify connection manager)

### Real-time Monitoring
- **Request Queue** - Pending request count
  - Source: Enhanced request queuing system
  - Effort: High (add request queue monitoring)
  
- **Active Locations** - Currently accessed URI paths
  - Source: Enhanced request tracking
  - Effort: High (add URI path monitoring)
  
- **Error Rate Trends** - Error percentage over time
  - Source: Enhanced error tracking with time series
  - Effort: High (add time-based error tracking)

---

## 📋 Implementation Checklist

### Phase 1: High Priority (Week 1)
- [ ] Add server name, version, protocol to ServerStatus
- [ ] Expose listening ports and hosts
- [ ] Add process ID and memory usage
- [ ] Add file descriptor count
- [ ] Update status page HTML to display new metrics

### Phase 2: Medium Priority (Week 2)
- [ ] Expose client max body size
- [ ] Add root and upload directory info
- [ ] Implement disk usage monitoring
- [ ] Add file count and size tracking
- [ ] Expose autoindex and CGI status

### Phase 3: Low Priority (Week 3+)
- [ ] Implement response time distribution
- [ ] Add bandwidth monitoring
- [ ] Enhance connection state tracking
- [ ] Add request queue monitoring
- [ ] Implement error rate trends

---

## 🛠️ Technical Implementation Notes

### Backend Changes Required
1. **ServerStatus Class Enhancement**
   - Add new member variables for additional metrics
   - Create getter methods for new information
   - Update JSON output to include new fields

2. **Configuration Exposure**
   - Pass config information to ServerStatus
   - Create methods to access server configurations
   - Aggregate data from multiple servers

3. **System Information Gathering**
   - Add system calls for resource monitoring
   - Implement periodic updates for changing metrics
   - Handle platform differences (Linux vs macOS)

### Frontend Updates
1. **HTML Structure**
   - Add new metric cards with appropriate styling
   - Maintain consistent design with existing elements
   - Ensure responsive layout for new metrics

2. **JavaScript Updates**
   - Parse new JSON fields from backend
   - Update DOM elements with new data
   - Handle missing or error data gracefully

---

## 🎯 Recommended Starting Point

**Start with Phase 1** - these features will give you immediate value with minimal effort:

1. **Server Identity** (5 minutes)
2. **Port Information** (15 minutes)  
3. **Process Info** (10 minutes)
4. **Memory Usage** (30 minutes)

This will transform your status page from basic metrics to a comprehensive server overview in under an hour of development time.

---

## 📚 Reference Files

- **Backend**: `srcs/core/server_status.cpp/hpp`
- **Frontend**: `www/status-page.html`, `www/js/status.js`
- **Configuration**: `srcs/parsing/Server.cpp`, `srcs/parsing/GlobalConfig.cpp`
- **Main Server**: `srcs/core/web_server.cpp`, `srcs/core/tcp_server.cpp`
