# webserver

## To do Lucas ## 
- [ ] use size limits in request Parsing
- [ ] move request parsing outside of Client connection?

## to implement ##
- [ ] Config parser
- [x] Core Server (NonBlocking I/O)
- [ ] Request parse (HttpRequest class)
- [ ] Response builder ()
- [ ] CGI
- [ ] Cookie

### Core server ###
- [x] Server initialization # from default config if none given
- [x] Non blocking and Event-driven I/O using epoll/select)
- [x] Socket connection


### config parser ###
- [ ] default params
- [ ] asterisk ?? (server_name *.example.com )
- [ ] remove comments
- [ ] refers to default.conf for each param to handle
- [ ]

### request parser ###
- [ ] methods (GET...)
- [ ] Host
- [ ] Parsing State
- [ ] Body
- [ ] feed class until all packet received
- [ ] handle weird char (\r\n) in image or pdf for example
- [ ]

### Response Builder ###
- [ ] status
- [ ] headers
- [ ] body
- [ ] content-type
- [ ] content-length
- [ ]

### CGI ###
- [ ]

### Cookie ###
- [ ]
