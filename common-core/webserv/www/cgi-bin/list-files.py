#!/usr/bin/env python3

import os
import sys
import json
from datetime import datetime
import time

# Print CGI headers
print("Content-Type: application/json")
print("Status: 200 OK")
print()  # Empty line separates headers from body

# Define uploads directory path
uploads_dir = os.environ["UPLOADS_DIR"]

# Initialize response data
response_data = {
    "success": False,
    "files": [],
    "error": None,
    "timestamp": datetime.now().isoformat()
}

try:
    # Check if uploads directory exists
    if not os.path.exists(uploads_dir):
        response_data["error"] = "Uploads directory does not exist"
        print(json.dumps(response_data, indent=2))
        sys.exit(1)

    files = []
    for filename in os.listdir(uploads_dir):
        file_path = os.path.join(uploads_dir, filename)
        if os.path.isfile(file_path):
            stat = os.stat(file_path)
            file_info = {
                "name": filename,
                "size": stat.st_size,
                "modified": datetime.fromtimestamp(stat.st_mtime).isoformat(),
                "created": datetime.fromtimestamp(stat.st_ctime).isoformat()
            }
            files.append(file_info)
    response_data["files"] = files
    response_data["success"] = True
    response_data["total_files"] = len(files)

except Exception as e:
    response_data["error"] = str(e)

# Output JSON response
print(json.dumps(response_data, indent=2)) 
