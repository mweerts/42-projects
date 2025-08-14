# webserver

## To discuss
- does the parsing protect from difference between number of cgi language and paths?
- make content-length not an int, it's not enough and could cause issue.

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

## To do Lucas ## 
- [ ] fix this "[ERROR] Error parsing request: 413 body size exceeds limit" when uploading
- [ ] handle transfer-encoding header and chunked request
- [/] Integrate uploads (POST) with FileWriteStream: wire client POLLIN → inBuf_ → file POLLOUT, with body completion flags and backpressure. // should be done but need to make sure
- [ ] Implemented non-blocking static file streaming via file FDs + poll; one-op-per-event.
- [ ] make a wrapper for send
- [ ] make saving the request to a file asynchronous too using the FileWriteStream

## Tests ##
* Tests:
	* [ ] Large static file GET.
	* [ ] Slow client (backpressure).
	* [ ] Large upload (multipart).
	* [ ] CGI that writes slowly/large output.
	* [ ] Keep-alive multiple sequential requests.
