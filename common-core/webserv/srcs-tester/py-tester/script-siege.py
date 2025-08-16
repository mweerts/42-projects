import subprocess
import requests
import time
url = "http://127.0.0.1:8080/"
urlApi = "http://127.0.0.1:8081/"
for i in range(100):
    result = subprocess.run(
        ["curl", "-s", "-o", "/dev/null", "-w", "%{http_code}", url],
        capture_output=True,
        text=True
    )
    result = subprocess.run(
        ["curl", "-s", "-o", "/dev/null", "-w", "%{http_code}", urlApi],
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

url = "http://127.0.0.1:8080/"
urlApi = "http://127.0.0.1:8081/"

for i in range(10):
    # Richiesta GET al primo URL con velocità limitata
    result = subprocess.run(
        ["curl", "--limit-rate", "50k", "-s", "-o", "/dev/null", "-w", "%{http_code}", url],
        capture_output=True,
        text=True
    )
    
    # Richiesta GET al secondo URL con velocità limitata
    resultApi = subprocess.run(
        ["curl", "--limit-rate", "50k", "-s", "-o", "/dev/null", "-w", "%{http_code}", urlApi],
        capture_output=True,
        text=True
    )
    
    # Estrazione del codice di risposta per il primo URL
    code = result.stdout.strip()
    
    if code == "200":
        # Verde per successo
        print(f"Request {i+1}: \033[92mOK\033[0m")
    else:
        # Rosso per errore
        print(f"Request {i+1}: \033[91mERROR {code}\033[0m")
    
    # Estrazione del codice di risposta per il secondo URL
    codeApi = resultApi.stdout.strip()
    
    if codeApi == "200":
        # Verde per successo
        print(f"Request {i+1} to API: \033[92mOK\033[0m")
    else:
        # Rosso per errore
        print(f"Request {i+1} to API: \033[91mERROR {codeApi}\033[0m")
    
    time.sleep(2)  # Attendi 2 secondi prima di inviare la prossima richiesta
#CURL SEND BIG FILE ###    
##CREATE A FILE 40mb ##
fileName = "file_40mb.txt"
size_mb = 40
chunk_size = 1024 * 1024  # 1 MB

with open(fileName, "wb") as f:
    for _ in range(size_mb):
        f.write(b'\x00' * chunk_size)
url = "http://127.0.0.1:8080/upload.html"
urlApi = "http://127.0.0.1:8081/upload.html"
file_path = "file_40mb.txt"

# Comando curl per upload multipart/form-data
cmd = [
    "curl",
    "-X", "POST",
    url,
    "-F", f"file=@{file_path}"
]
cmdApi = [
    "curl",
    "-X", "POST",
    urlApi,
    "-F", f"file=@{file_path}"
]

# Esegui il comando
result = subprocess.run(cmd, capture_output=True, text=True)

# Output (debug)
print("## TEST POST IN ", url)
print("Status code:", result.returncode)
print("STDOUT:\n", result.stdout)
print("STDERR:\n", result.stderr)

result = subprocess.run(cmdApi, capture_output=True, text=True)

# Output (debug)
print("## TEST POST IN ", urlApi)
print("Status code:", result.returncode)
print("STDOUT:\n", result.stdout)
print("STDERR:\n", result.stderr)
### SLOW_OUTPUT ###

print("### TEST CGI SLOW OUTPUT ###")
subprocess.run(["curl", "http://127.0.0.1:8080/cgi-bin/slow_output.py"])

## SLEEP AND TEST TO SIEGE ##
cmdMini = ["siege", url, "-c", "2","-v", "-d", "0.5", "-t", "10s"]
cmdFull = ["siege", "-f","urlFull.txt" , "-c", "10", "-t", "10s"]
cmdBasic = ["siege", "-f","urlBasic.txt" , "-c", "10", "-t", "10s"]


######siege -c 5 -r 10 --delay=2 -t 1m http://127.0.0.1:8080/large_file.txt
cmdLow = ["siege", "-c", "5", "-r", "10", "--delay", "2", "-t", "1m", url]
time.sleep(5)
print("### TEST SIEGE ####")
print("### LOWPRESSURE SIEGE ####")
subprocess.run(cmdLow, capture_output=False)
print("### MINI TEST SIEGE ####")
subprocess.run(cmdMini, capture_output=False)
print("### FULL TEST SIEGE ####")
time.sleep(1)
subprocess.run(cmdFull, capture_output=False)
print("### TEST JUST ALLOWMETODH ####")
time.sleep(1)
subprocess.run(cmdBasic, capture_output=False)

#subprocess.run(cmd, capture_output=False)

