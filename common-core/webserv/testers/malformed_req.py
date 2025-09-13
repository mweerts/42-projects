#!/usr/bin/env python3
import socket

def recv_all(sock):
    data = b""
    while b"\r\n\r\n" not in data:
        chunk = sock.recv(4096)
        if not chunk:
            return data
        data += chunk

    headers, rest = data.split(b"\r\n\r\n", 1)
    content_length = None
    for line in headers.split(b"\r\n")[1:]:
        k, sep, v = line.partition(b":")
        if k.lower() == b"content-length":
            try:
                content_length = int(v.strip())
            except ValueError:
                pass

    if content_length is not None:
        body = rest
        while len(body) < content_length:
            chunk = sock.recv(4096)
            if not chunk:
                break
            body += chunk
        return headers + b"\r\n\r\n" + body

    body = rest
    while True:
        chunk = sock.recv(4096)
        if not chunk:
            break
        body += chunk
    return headers + b"\r\n\r\n" + body

def test_request(request):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('localhost', 8080))
    s.sendall(request)
    resp = recv_all(s)
    s.close()
    print(f"Request: {request!r}")
    print(f"Response: {resp.decode(errors='replace')}")
    print("---")

test_requests = [
    b"GET / HTTP/1.1\r\n\r\n",
    b"POST / HTTP/1.1\r\n\r\n",
	
    # Test invalid methods
    b"INVALID / HTTP/1.1\r\n\r\n",
    b"123 / HTTP/1.1\r\n\r\n",
    
    # Test invalid versions
    b"GET / HTTP/1.5\r\n\r\n",
    b"GET / HTTP/2.0\r\n\r\n",
    b"GET / HTTP/INVALID\r\n\r\n",
	b"GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n",
    b"INVALID / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n",
    b"INVALID / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n",
    b"GET / HTTP/1.5\r\nHost: localhost:8080\r\n\r\n",
    b"GET HTTP/1.1\r\nHost: localhost:8080\r\n\r\n",
    b"GET / HTTP/\r\nHost: localhost:8080\r\n\r\n",
    b"INVALID REQUEST\r\n\r\n",

    # Test incomplete requests
    b"GET / HTTP/1.1\r\n"
]

for req in test_requests:
    test_request(req)
