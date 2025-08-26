#!/usr/bin/env python3
import socket

def test_request(request):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(('localhost', 8080))
    sock.send(request)
    response = sock.recv(1024).decode()
    sock.close()
    
    print(f"Request: {repr(request)}")
    print(f"Response: {response}")
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
