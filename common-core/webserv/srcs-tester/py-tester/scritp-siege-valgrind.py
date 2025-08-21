import subprocess
import time
import sys
import signal

url = "http://127.0.0.1:8080/"
urlApi = "http://127.0.0.1:8081/"
MAX_REQUEST_CURL = 100;
MAX_CURL_SLOW = 10;
fileName = "file_40mb.txt"
size_mb = 40 #♡♡♡♡♡♡♡♡♡♡♡ITERATORE CICLO CREAZIONE BIF FILE 
chunk_size = 1024 * 1024  # 1 MB

with open(fileName, "wb") as f:
    for _ in range(size_mb):
        f.write(b'\x00' * chunk_size)

config_list = [
    "../../config/TestConf/JusteServer.conf",
    "../../config/default2.conf",
#    "../../config/TestConf/HeritanceBody.conf",
#    "../../config/TestConf/noErrorPage.conf",
    "../../config/TestConf/noBindPort.conf",
    "../../config/TestConf/1kb.conf",
    "../../config/TestConf/portNameLocalHost.conf",
#    "../../config/TestConf/HeritanceErrorPage.conf",
    "../../config/TestConf/noUploadDir.conf"
#    "../../config/TestConf/3Server.conf",
]
parser_list = [
    "../../config/ParserConf/noRootServer.conf",
    "../../config/ParserConf/noRootLocation.conf",
    "../../config/ParserConf/noRootCGI.conf",
    "../../config/ParserConf/noPort.conf",
    "../../config/ParserConf/noCgiBin.conf",
    "../../config/ParserConf/ForbitenPrmtrsinLocation00.conf",
    "../../config/ParserConf/ForbitenPrmtrsinLocation01.conf",
    "../../config/ParserConf/ForbitenPrmtrsinLocation02.conf",
    "../../config/ParserConf/ForbitenPrmtrsinCGI00.conf",
    "../../config/ParserConf/ForbitenPrmtrsinCGI01.conf",
    "../../config/ParserConf/ForbitenPrmtrsinCGI02.conf",
    "../../config/ParserConf/noCgiBin.conf",
    "../../config/ParserConf/noValadePort.conf",
    "../../config/ParserConf/samePortSever.conf"
    #"../../config/ParserConf/badSyntax.conf"
    #"../../config/ParserConf/isAdir.conf"
    #"../../config/ParserConf/no.conf.rand"

]

##♡♡♡♡♡♡♡♡♡♡♡TESTIAMO IL PARSER TUTTE IL SERVER NON SI DEVE AVVIARE♡♡♡♡♡♡♡♡♡♡♡
i = 0;
while i < (len(parser_list)):
    print(f"\n==== PARSER TEST {i+1} ====")
    print(parser_list[i])
    cmdValgrind = ["valgrind", "../../webserv", "-c", parser_list[i]]
    process = subprocess.Popen(cmdValgrind)
    time.sleep(2)
    for l in range(len(parser_list)):
        print(parser_list[l], f"index is {l}")
    cont  = input("Touch me for continue...")
    if (cont == "kill"):
        process.send_signal(signal.SIGINT)
        exit();
    if (cont == "break"):
        break ;
    if cont == "":
        i += 1
    else:
        try:
            new_index = int(cont)
            if 0 <= new_index < len(parser_list):
                i = new_index
            else:
                print(" Indice fuori range. Continuo al prossimo.")
                i += 1
        except ValueError:
            print(" Input non valido. Continuo al prossimo.")
            i += 1
    process.send_signal(signal.SIGINT)

ciclo = 0;    
while ciclo < len(config_list):

    print(f"\n==== INIZIO CICLO {ciclo+1} ====")
    url = "http://127.0.0.1:8080/"
    urlApi = "http://127.0.0.1:8081/"

    print("### AVVIO VALGRIND ###")
    cmdValgrind = ["valgrind", "../../webserv", "-c", config_list[ciclo]]
    proc = subprocess.Popen(cmdValgrind)
    time.sleep(2)
    print(config_list[ciclo])
    cont  = input("Touch me for continue...")
    if (cont == "kill"):
        proc.send_signal(signal.SIGINT)
        exit();
    if (cont == "continue"):
        proc.send_signal(signal.SIGINT)
        continue ;
##♡♡♡♡♡♡♡♡♡♡♡100 RIHIESTE CURL VELOCI TIME REALE♡♡♡♡♡♡♡♡♡♡♡
    for i in range(MAX_REQUEST_CURL):
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
        #time.sleep(0.5);


#♡♡♡♡♡♡♡♡♡♡♡LIMITAZIONE MANDA SIMULAZIONE UTENTI LENTI♡♡♡♡♡♡♡♡♡♡♡
    for i in range(MAX_CURL_SLOW):
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

    url = "http://127.0.0.1:8080/upload"
    urlApi = "http://127.0.0.1:8081/upload"

#♡♡♡♡♡♡♡♡♡♡♡CURL SEND BIG FILE ###♡♡♡♡♡♡♡♡♡♡♡    
##CREATE A FILE 40mb ##
    fileName = "file_40mb.txt"
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

    url = "http://127.0.0.1:8080/"
###♡♡♡♡♡♡♡♡♡♡♡ SLOW_OUTPUT♡♡♡♡♡♡♡♡♡♡♡ ###

#TODO: RIEMPIRE IL BUFFER DI 1GB VEDER SE SALTA TUTTO♡♡♡♡♡♡♡♡♡♡♡
    print("### TEST CGI SLOW OUTPUT ###")
    subprocess.run(["curl", "http://127.0.0.1:8080/cgi-bin/slow_output.py"])

##♡♡♡♡♡♡♡♡♡♡♡ SLEEP AND TEST TO SIEGE♡♡♡♡♡♡♡♡♡♡♡ ##
    cmdMini = ["siege", "-f","urlBasic.txt", "-c", "2", "-v", "-d", "0.5", "-t", "10s"]
    cmdFull = ["siege", "-f", "urlFull.txt", "-c", "10", "-t", "10s"]
    cmdBasic = ["siege", "-f", "urlBasic.txt", "-c", "10", "-t", "10s"]
    cmdBanch = ["siege", "-b", "-f", "urlBasic.txt", "-t", "15s"]
    cmdLow = ["siege", "-c", "5", "-r", "10", "--delay", "2", "-t", "1m", url]

    time.sleep(5)
    print("### TEST SIEGE ####")
    print("### LOW PRESSURE SIEGE ####")
    cont  = input("Touch me for continue...")
    if (cont == "kill"):
        proc.send_signal(signal.SIGINT)
        exit();
    if (cont == "continue"):
        proc.send_signal(signal.SIGINT)
        continue ;
        
        
#######♡♡♡♡♡♡♡♡♡♡♡  SUB PROCCESS SIEGE♡♡♡♡♡♡♡♡♡♡♡
    subprocess.run(cmdLow)
    time.sleep(1)
    print("### MINI TEST SIEGE ####")
    input("Touch me for continue...")
    if (cont == "kill"):
        proc.send_signal(signal.SIGINT)
        exit();
    if (cont == "continue"):
        proc.send_signal(signal.SIGINT)
        continue;
    subprocess.run(cmdMini)
    time.sleep(1)
    print("### FULL TEST SIEGE ####")
    input("Touch me for continue...")
    if (cont == "kill"):
        proc.send_signal(signal.SIGINT)
        exit();
    if (cont == "continue"):
        proc.send_signal(signal.SIGINT)
        continue;
    subprocess.run(cmdFull)
    time.sleep(1)
    print("### TEST JUST ALLOWMETHOD ####")
    input("Touch me for continue...")
    if (cont == "kill"):
        proc.send_signal(signal.SIGINT)
        exit();
    if (cont == "continue"):
        proc.send_signal(signal.SIGINT)
        continue;
    subprocess.run(cmdBasic)
    time.sleep(1)
    print("### TEST BENCHMARK ####")
    input("Touch me for continue...")
    if (cont == "kill"):
        proc.send_signal(signal.SIGINT)
        exit();
    if (cont == "continue"):
        proc.send_signal(signal.SIGINT)
        continue;
    subprocess.run(cmdBanch)

    # ♡♡♡♡♡♡♡♡♡♡♡End Valgrind (test più esterno - dopo tutto)♡♡♡♡♡♡♡♡♡♡♡
    print("### FINE VALGRIND ###")

    print(f"==== FINE CICLO {ciclo+1} ====")
    print("Invio Ctrl+C al processo...")
    proc.send_signal(signal.SIGINT)
    time.sleep(2);
    for y in range(len(config_list)):
        print(config_list[y], f"index is {y}:")
    cont = input("Touch me for continue...")
    if (cont == "kill"):
        exit();
    if cont == "":
        ciclo += 1  # continua normalmente
    else:
        try:
            new_index = int(cont)
            if 0 <= new_index < len(config_list):
                ciclo = new_index
            else:
                print(" Indice fuori range. Continuo al prossimo.")
                ciclo += 1
        except ValueError:
            print(" Input non valido. Continuo al prossimo.")
            ciclo += 1

