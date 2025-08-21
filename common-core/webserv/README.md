# webserver

## To discuss

- does the parsing protect from difference between number of cgi language and paths?
- make content-length not an int, it's not enough and could cause issue.
- location is broken with autodindex not working, it works by default not when the location is configured (je me rappelle plus trop comment tester ca)

## to do max

- Location doit etre dynamique dans process request, la c'est tout le temps localhost:8080
- Fix le soucis avec return

## To do cgi

- Upload streaming:
  - Wire FileWriteStream into the event loop:
    - Client POLLIN → inBuf\_ → file POLLOUT (one write per ready event).
    - When body complete and inBuf\_ empty → close file → send 201/204.
- Error mapping polish:
  - File/CGI failures → precise HTTP codes (404/403/500/507), consistent default error pages.

## To do Lucas

- [URGENT] REFACTOR PARSE REQUEST TO ONLY SAVE THE REQUEST TO A FILE WHEN NECESSARY
- [URGENT] REFACTOR PARSE REQUEST TO ONLY SAVE THE REQUEST TO A FILE WHEN NECESSARY
- [URGENT] REFACTOR PARSE REQUEST TO ONLY SAVE THE REQUEST TO A FILE WHEN NECESSARY

- [ ] make better pages for files and add option to delete from page

// should be done but need to make sure

- [/] Integrate uploads (POST) with FileWriteStream: wire client POLLIN → inBuf\_ → file POLLOUT, with body completion flags and backpressure.

## Tests

- Tests:
  - [ ] Large static file GET.
  - [ ] Slow client (backpressure).
  - [ ] Large upload (multipart).
  - [ ] CGI that writes slowly/large output.
  - [ ] Keep-alive multiple sequential requests.
