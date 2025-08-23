import os
import sys

uploads_dir = os.environ["UPLOADS_DIR"]
query_string = os.environ["QUERY_STRING"]

filename = query_string.split("=")[1]

if not os.path.exists(uploads_dir):
    print("Uploads directory does not exist")
    sys.exit(1)

host = os.environ["HOST"]

url = f"http://{host}/{uploads_dir}/{filename}"

os.system(f"curl -X DELETE {url}")

print("File deleted")
