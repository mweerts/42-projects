# webserver

## To discuss
- right now the cgi is non blocking but not truely asynchronous, should we still implement it? (means more state and complexity as well as more potential bugs)
- does the parsing protect from difference between number of cgi language and paths?

## To do cgi 

* Upload streaming:
	* Wire FileWriteStream into the event loop:
		* Client POLLIN → inBuf_ → file POLLOUT (one write per ready event).
		* When body complete and inBuf_ empty → close file → send 201/204.
* Backpressure toggling:
	* Use StreamBuffer watermarks to dynamically enable/disable interests:
		* If outBuf_ is “full”: stop polling file/CGI stdout (pause producer).
		* If inBuf_ is “full”: stop polling client POLLIN (pause client reads).
* CGI input streaming (optional but better):
	* Instead of readBodyAll() once, feed CGI stdin incrementally from the request body using the same buffer/backpressure logic.
* Timeouts:
	Re-add CGI timeout check during response phase (e.g., on each poll tick) and kill the child if exceeded.
* Error mapping polish:
	* File/CGI failures → precise HTTP codes (404/403/500/507), consistent default error pages.
* Remove/avoid legacy direct-send path:
	* Treat RequestHandler::sendResponse() as deprecated; all responses should go through the partial-send/streaming pipeline 	you now have.
* Tests:
	* Large static file GET.
	* Slow client (backpressure).
	* Large upload (multipart).
	* CGI that writes slowly/large output.
	* Keep-alive multiple sequential requests.

## To do Lucas ## 
- [ ] handle transfer-encoding header?
- [ ] Integrate uploads (POST) with FileWriteStream: wire client POLLIN → inBuf_ → file POLLOUT, with body completion flags and backpressure.
- [ ] Add buffer thresholds (64KB/16KB) to enable/disable interests dynamically.
- [ ] Implemented non-blocking static file streaming via file FDs + poll; one-op-per-event.
- [ ] Added header-only send and partial body sending.
- [ ] Extended poll loop to include aux FDs and route events back to connections.


### Core server ###
- [x] Server initialization # from default config if none given
- [x] Non blocking and Event-driven I/O using epoll/select
- [x] Socket connection

### CGI ###
- [ ]

### Cookie ###
- [ ]
