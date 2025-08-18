import subprocess
import time
import signal

url = "http://127.0.0.1:8080/"
urlApi = "http://127.0.0.1:8081/"
fileName = "file_40mb.txt"
size_mb = 40
chunk_size = 1024 * 1024  # 1 MB
config_list = [
        "../../config/default2.conf",
    "../../config/TestConf/noErrorPage.conf",
    "../../config/TestConf/noBindPort.conf",
    "../../config/TestConf/JusteServer.conf",
    "../../config/TestConf/1kb.conf"
]
# Creo il file una volta sola fuori dal ciclo
with open(fileName, "wb") as f:
    for _ in range(size_mb):
        f.write(b'\x00' * chunk_size)

for ciclo in range(5):  # Puoi cambiare 10 con quante iterazioni vuoi

    print(f"\n==== INIZIO CICLO {ciclo+1} ====")

    # Start Valgrind (test più esterno - prima)
    print("### AVVIO VALGRIND ###")
    cmdValgrind = ["valgrind", "../../webserv", "-c", config_list[ciclo]]
    proc = subprocess.Popen(cmdValgrind)
    time.sleep(4)

    # Primo gruppo di richieste curl senza limitazione (100 richieste)
    for i in range(100):
        result = subprocess.run(
            ["curl", "-s", "-o", "/dev/null", "-w", "%{http_code}", url],
            capture_output=True,
            text=True
        )
        resultApi = subprocess.run(
            ["curl", "-s", "-o", "/dev/null", "-w", "%{http_code}", urlApi],
            capture_output=True,
            text=True
        )
        code = result.stdout.strip()
        codeApi = resultApi.stdout.strip()

        print(f"Request {i+1}: \033[92mOK\033[0m" if code == "200" else f"Request {i+1}: \033[91mERROR {code}\033[0m")
        print(f"RequestApi {i+1}: \033[92mOK\033[0m" if codeApi == "200" else f"RequestApi {i+1}: \033[91mERROR {codeApi}\033[0m")
        time.sleep(0.5);

    # Secondo gruppo di richieste curl con limitazione di banda (10 richieste)
    for i in range(10):
        result = subprocess.run(
            ["curl", "--limit-rate", "50k", "-s", "-o", "/dev/null", "-w", "%{http_code}", url],
            capture_output=True,
            text=True
        )
        resultApi = subprocess.run(
            ["curl", "--limit-rate", "50k", "-s", "-o", "/dev/null", "-w", "%{http_code}", urlApi],
            capture_output=True,
            text=True
        )
        code = result.stdout.strip()
        codeApi = resultApi.stdout.strip()

        print(f"Request {i+1}: \033[92mOK\033[0m" if code == "200" else f"Request {i+1}: \033[91mERROR {code}\033[0m")
        print(f"RequestApi {i+1}: \033[92mOK\033[0m" if codeApi == "200" else f"RequestApi {i+1}: \033[91mERROR {codeApi}\033[0m")

        time.sleep(2)

    # Upload file (40 MB)
    cmdUpload = [
        "curl",
        "-X", "POST",
        url,
        "-F", f"file=@{fileName}"
    ]
    cmdUploadApi = [
        "curl",
        "-X", "POST",
        urlApi,
        "-F", f"file=@{fileName}"
    ]
    result = subprocess.run(cmdUpload, capture_output=True, text=True)
    print(f"## TEST POST IN {url}")
    print("Status code:", result.returncode)
    print("STDOUT:\n", result.stdout)
    print("STDERR:\n", result.stderr)

    result = subprocess.run(cmdUploadApi, capture_output=True, text=True)
    print(f"## TEST POST IN {urlApi}")
    print("Status code:", result.returncode)
    print("STDOUT:\n", result.stdout)
    print("STDERR:\n", result.stderr)

    # Slow output test
    print("### TEST CGI SLOW OUTPUT ###")
    subprocess.run(["curl", "http://127.0.0.1:8080/cgi-bin/slow_output.py"])

    # Siege tests
    cmdMini = ["siege", url, "-c", "2", "-v", "-d", "0.5", "-t", "10s"]
    cmdFull = ["siege", "-f", "urlFull.txt", "-c", "10", "-t", "10s"]
    cmdBasic = ["siege", "-f", "urlBasic.txt", "-c", "10", "-t", "10s"]
    cmdBanch = ["siege", "-b", "urlBasic.txt"]
    cmdLow = ["siege", "-c", "5", "-r", "10", "--delay", "2", "-t", "1m", url]

    time.sleep(5)
#    print("### TEST SIEGE ####")
#    print("### LOW PRESSURE SIEGE ####")
#    subprocess.run(cmdLow)
#    time.sleep(1)
#    print("### MINI TEST SIEGE ####")
#    subprocess.run(cmdMini)
#    time.sleep(1)
#    print("### FULL TEST SIEGE ####")
#    subprocess.run(cmdFull)
#    time.sleep(1)
#    print("### TEST JUST ALLOWMETHOD ####")
#    subprocess.run(cmdBasic)
#    time.sleep(1)
#    print("### TEST BENCHMARK ####")
#    subprocess.run(cmdBanch)
#
#    # End Valgrind (test più esterno - dopo tutto)
#    print("### FINE VALGRIND ###")
#    subprocess.run(cmdValgrind)
#
#    print(f"==== FINE CICLO {ciclo+1} ====")
    print("Invio Ctrl+C al processo...")
    proc.send_signal(signal.SIGINT)
    time.sleep(5)

