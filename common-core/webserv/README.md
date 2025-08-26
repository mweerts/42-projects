# webserver

## To discuss

- make content-length not an int, it's not enough and could cause issue.

## to do max

- Fix le soucis avec return

## To do cgi


// blocking right now
// but need a not a small refactor..
* - Upload streaming:
  * - Wire FileWriteStream into the event loop:
  * - [ ] Integrate uploads (POST) with FileWriteStream: wire client POLLIN → inBuf\_ → file POLLOUT, with body 		completion flags and backpressure. 

## To do Lucas


## Tests

- Tests:
  - [ ] Large static file GET.
  - [ ] Slow client (backpressure).
  - [ ] Large upload (multipart).
  - [ ] CGI that writes slowly/large output.
  - [ ] Keep-alive multiple sequential requests.
