#!/usr/bin/env python3

import os
import sys

# CGI Headers - MUST come first
print("Content-Type: text/html")
print("Status: 200 OK")
print()  # Empty line separates headers from body

# HTML Response
print("""<!DOCTYPE html>
<html>
<head>
    <title>CGI Test - GET</title>
</head>
<body>
    <h1>CGI GET Test Successful!</h1>
    <h2>Environment Variables:</h2>
    <ul>""")

# Display key CGI environment variables
cgi_vars = [
    'REQUEST_METHOD', 'QUERY_STRING', 'CONTENT_LENGTH', 
    'CONTENT_TYPE', 'SCRIPT_NAME', 'PATH_INFO',
    'SERVER_NAME', 'SERVER_PORT', 'SERVER_PROTOCOL'
]

for var in cgi_vars:
    value = os.environ.get(var, 'Not set')
    print(f"        <li><strong>{var}:</strong> {value}</li>")

print("""    </ul>
    <h2>Query String Processing:</h2>
    <p>Query String: <strong>{}</strong></p>
    <p>Working Directory: <strong>{}</strong></p>
</body>
</html>""".format(
    os.environ.get('QUERY_STRING', 'None'),
    os.getcwd()
))
