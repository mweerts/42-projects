# webserver

## To do Lucas ## 
- [] replace http_servers by a map instead of vector in core

## to implement ##
- [] Config parser
- [] Core Server (NonBlocking I/O)
- [] Request parse (HttpRequest class)
- [] Response builder ()
- [] CGI
- [] Cookie

### Core server ###
- [] Server initialization # from default config if none given
- [] Non blocking and Event-driven I/O using epoll/select)
- [] Socket connection
- [] 


### config parser ###
- [] default params
- [] asterisk ?? (server_name *.example.com )
- [] remove comments
- [] refers to default.conf for each param to handle
- []

### request parser ###
- [] methods (GET...)
- [] Host
- [] Parsing State
- [] Body
- [] feed class until all packet received
- [] handle weird char (\r\n) in image or pdf for example
- []

### Response Builder ###
- [] status
- [] headers
- [] body
- [] content-type
- [] content-length
- []

### CGI ###
- []

### Cookie ###
- []
