# webserver

## To discuss
- right now the cgi is non blocking but not truely asynchronous, should we still implement it? (means more state and complexity as well as more potential bugs)

## To do
- does the parsing protect from difference between number of cgi language and paths?

## To do Lucas ## 
- [ ] handle transfer-encoding header?

### Core server ###
- [x] Server initialization # from default config if none given
- [x] Non blocking and Event-driven I/O using epoll/select
- [x] Socket connection

### CGI ###
- [ ]

### Cookie ###
- [ ]
