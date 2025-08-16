import subprocess
import time
url = "http://127.0.0.1:8080/"
for i in range(1000):
    result = subprocess.run(
        ["curl", "-s", "-o", "/dev/null", "-w", "%{http_code}", url],
        capture_output=True,
        text=True
    )
    result = subprocess.run(
        ["curl", "-s", "-o", "/dev/null", "-w", "%{http_code}", url],
        capture_output=True,
        text=True
        )

    code = result.stdout.strip()

    if code == "200":
        # Verde
        print(f"Request {i+1}: \033[92mOK\033[0m")
    else:
        # Rosso
        print(f"Request {i+1}: \033[91mERROR {code}\033[0m")
time.sleep(2)
#CURL SEND BIG FILE ###    
##CREATE A FILE 40mb ##
fileName = "file_40mb.bin"
size_mb = 40
chunk_size = 1024 * 1024  # 1 MB

with open(fileName, "wb") as f:
    for _ in range(size_mb):
        f.write(b'\x00' * chunk_size)
#result = subprocess.run()
## SLEEP AND TEST TO SIEGE ##
time.sleep(5)
cmdMini = ["siege", url, "-c", "1", "-t", "10s"]
cmdFull = ["siege", url, "-c", "10", "-t", "10s"]
print("### TEST SIEGE ####")
print("### MINI TEST SIEGE ####")
subprocess.run(cmdMini, capture_output=False)
print("### FULL TEST SIEGE ####")

#subprocess.run(cmd, capture_output=False)

