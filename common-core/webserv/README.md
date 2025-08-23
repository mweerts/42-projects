# webserver

## To discuss

- [ ] make DELETE disallowed by default? (it's the case for apache or nginx)
- [ ] DELETE not protected, i was able to delete webserver.log with this in config 
```
location / {
    allow_methods GET POST;
  }
```
- make content-length not an int, it's not enough and could cause issue.

## to do max

- Location doit etre dynamique dans process request, la c'est tout le temps localhost:8080
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
