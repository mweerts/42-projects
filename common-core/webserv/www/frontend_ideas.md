# Frontend Ideas for Home Page Replacement

## Problem
Having the same upload section on both home page and upload page is redundant. Need alternatives that require minimal server-side work.

## Low-Effort Options (Minimal Backend Required)

### 1. File Browser (Easiest) <!-- need to implement this -->
- **Just needs**: Directory listing (which you probably already have)
- **Simple CGI**: `ls -la /uploads/` or similar <!-- or maybe json format if easier for frontend -->
- **Frontend**: JavaScript to parse and display the output
- **Features**: File list, sizes, dates, click to download


### 2. System and server Health Check <!-- also good idea for cpu / memory etc -->
- **Just needs**: Basic system commands
- **Simple CGI**: `free -m`, `df -h`, `ps aux | wc -l`, `uptime`, `ps aux | grep webserv`
- **Frontend**: Parse command output and display nicely, memory, disk, process info, 
- **Features**: Simple health indicators


### 3. Simple Log Viewer <!-- this is one the best idea for now ! -->
- **Just needs**: Read access to log files
- **Simple CGI**: `tail -n 20 /path/to/webserv.log`
- **Frontend**: Display log entries with syntax highlighting
- **Features**: Recent requests, error logs, access logs

### 4. Configuration Display <!-- also nice -->
- **Just needs**: Read config file
- **Simple CGI**: `cat /path/to/config.conf`
- **Frontend**: Parse and display config in a nice format
- **Features**: Show current server settings, routes, etc.


## Higher-Effort Options (Would Need More Backend Work)

### 5. Server Status Dashboard <!-- a simple get request to specific endpoint might do the trick but not sure-->
- **Real-time stats**: Active connections, requests/second, uptime
- **Performance metrics**: CPU usage, memory, bandwidth
- **Live logs**: Recent HTTP requests with status codes
- **Health indicators**: Server status, active connections, error rates
- **example**:
```
// just need to protect if location already exists
if (request.path == "/api/status") {
    // Return JSON with server stats
    std::string json = "{\"uptime\":\"" + getUptime() + "\",\"connections\":" + std::to_string(activeConnections) + "}";
    response.setBody(json);
    response.setHeader("Content-Type", "application/json");
}
```

### 7. API Testing Interface
- **HTTP method tester**: GET, POST, DELETE with custom headers
- **Response viewer**: Pretty-printed JSON/HTML responses
- **Request builder**: URL builder with query parameters
- **Status code checker**: Test different endpoints

### 11. Interactive Demo
- **Step-by-step tutorial**: How to use the server
- **Interactive examples**: Click to test different features
- **Feature showcase**: Highlight key capabilities
- **Getting started guide**

