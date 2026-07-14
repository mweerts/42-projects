import subprocess
import time
import sys
import signal
import os

url = "http://127.0.0.1:8080/"
urlApi = "http://127.0.0.1:8081/"
urlup = "http://127.0.0.1:8080/upload"
urlupApi = "http://127.0.0.1:8081/upload"
MAX_CURL_SLOW = 10;
MAX_REQUEST_CURL = 100;
MSG_B_T = "### TEST BENCHMARK ####";
MSG_J_T = "### TEST JUST ALLOWMETHOD ####";
MSG_F_T = "### FULL TEST SIEGE ####";
MSG_L_T ="### LOW PRESSURE SIEGE ####";
MSG_M_T ="### MINI TEST SIEGE ####";
MSG_C_T = "##### CGI TEST SIEGE ####";
fileName = "file_40mb.txt"
size_mb = 40 #♡♡♡♡♡♡♡♡♡♡♡ITERATORE CICLO CREAZIONE BIF FILE 
chunk_size = 1024 * 1024  # 1 MB
cmdMini = ["siege", "-f","urlBasic.txt", "-c", "2", "-v", "-d", "0.5", "-t", "10s"]
cmdFull = ["siege", "-f", "urlFull.txt", "-c", "10", "-t", "10s"]
cmdBasic = ["siege", "-f", "urlBasic.txt", "-c", "10", "-t", "10s"]
cmdSlowCgi = ["siege", "-f", "urlBasic.txt", "-c", "10", "-t", "10s"]
cmdMaxC = ["siege", "-f", "urlBasic.txt", "-c", "200", "-t", "25s"]
cmdCGI = ["siege", "-f", "urlCGI.txt", "-c", "10", "-t", "25s"]
cmdBanch = ["siege", "-b", "-f", "urlBasic.txt", "-t", "20s"]
cmdLow = ["siege", "-c", "5", "-r", "10", "--delay", "2", "-t", "1m", url]

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
#    "../../config/TestConf/JustRootServerNoName.conf",
#    "../../config/TestConf/10Location.conf",
]
parser_list = [
    "../../config/ParserConf/noRootServer.conf",
    "../../config/ParserConf/noRandomBracketClose.conf",
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
    "../../config/ParserConf/samePortSever.conf",
    "../../config/ParserConf/badSyntax.conf",
    "../../config/ParserConf/badSyntax01.conf",
    "../../config/ParserConf/badSyntax02.conf",
    "../../config/ParserConf/isAdir.conf",
    "../../config/ParserConf/no.conf.conf.conf.",
    "../../config/ParserConf/justEmpty.conf",
    "../../config/ParserConf/noAccessRoot.conf",
    "../../config/ParserConf/justComment.conf "

]

def ft_startGeneratorTxtSiege(index):
    cmdGeneratorTxt = ["./GeneratoreTester", config_list[index]]
    subprocess.run(cmdGeneratorTxt, capture_output = True)
    
def ft_cmdGenerator(index):
        return ["valgrind", "--track-fds=yes", "../../webserv", "-c", config_list[index]]

def ft_create_big_file():
    with open(fileName, "wb") as f:
        for _ in range(size_mb):
            f.write(b'\x00' * chunk_size)

def ft_input_index(i):
    cont = input("Touch me for continue... or kill or break")

    if cont == "kill":
        return -1;
    elif cont == "break":
        return -2;
    elif cont == "":
        return i + 1;
    else:
        try:
            new_index = int(cont)
            if 0 <= new_index < len(parser_list):
                return (new_index)
            else:
                print("Indice fuori range. Continuo al prossimo.")
                return (i + 1)
        except ValueError:
            print("Input non valido. Continuo al prossimo.")
            return (i + 1)


def ft_print_list(lst):
    for i in range(len(lst)):
        print(lst[i], f"index is: {i}")
    
##♡♡♡♡♡♡♡♡♡♡♡TESTIAMO IL PARSER TUTTE IL SERVER NON SI DEVE AVVIARE♡♡♡♡♡♡♡♡♡♡♡
def ft_parsing(start_index):
    i = start_index;
    while i < (len(parser_list)):
        print(f"\n==== PARSER TEST {i+1} ====")
        print(parser_list[i])
        cmdValgrind = ["valgrind", "../../webserv", "-c", parser_list[i]]
        process = subprocess.Popen(cmdValgrind)
        time.sleep(2)
        ft_print_list(parser_list)
        i = ft_input_index(i);
        process.send_signal(signal.SIGINT)
        if (i < 0):
            if (i == -1):
                exit()
            elif (i == -2):
                 break

def ft_input(proc):
    cont  = input("Touch me for continue... or break or continue")
    if (cont == "break"):
        proc.send_signal(signal.SIGINT)
        return -1;
    if (cont == "continue"):
        proc.send_signal(signal.SIGINT)
        return -2;
    return 0;

def ft_print_error_curl(code, i):
    print(f"Request {i+1}: \033[92mOK\033[0m" if code == "200" else f"Request {i+1}: \033[91mERROR {code}\033[0m")

def ft_real_time_curl(url, i):
    result = subprocess.run(
        ["curl", "-s", "-o", "/dev/null", "-w", "%{http_code}", url],
        capture_output=True,
        text=True
    )
    code = result.stdout.strip()
    ft_print_error_curl(code, i);


def ft_limit_rate_curl(url, i):
    result = subprocess.run(
            ["curl", "--limit-rate", "50k", "-s", "-o", "/dev/null", "-w", "%{http_code}", url],
            capture_output=True,
            text=True
            )
    code = result.stdout.strip()
    ft_print_error_curl(code, i);

def ft_post_curl(urlA, fileToName):
    cmdUpload = [
        "curl",
        "-X", "POST",
        urlA,
        "-F", f"file=@{fileToName}"
    ]
    print("DEBUG cmdUpload =", cmdUpload)
    print("DEBUG types =", [type(x) for x in cmdUpload])

    result = subprocess.run(cmdUpload, capture_output=True, text=True)
    print(f"## TEST POST IN {urlA}")
    print("Status code:", result.returncode)
    print("STDOUT:\n", result.stdout)
    print("STDERR:\n", result.stderr)
    


def ft_for_func(rng, ft1, ft2, info1, info2 ):
    for i in range(rng):
        ft1(info1, i);
        ft2(info2, i);



def ft_siege(cmd, strPrint, sleepTime):
    subprocess.run(cmd)
    print(strPrint)
    time.sleep(sleepTime)

def ft_full_test(start_index):
    ciclo = start_index;    
    while ciclo < len(config_list):
    
        print(f"\n==== INIZIO CICLO {ciclo+1} ====")
        url = "http://127.0.0.1:8080/"
        urlApi = "http://127.0.0.1:8081/"
        print("### AVVIO VALGRIND ###")
        ft_startGeneratorTxtSiege(ciclo);
        proc = subprocess.Popen(ft_cmdGenerator(ciclo))
        time.sleep(2)
        print(config_list[ciclo])
        ft_for_func(10, ft_limit_rate_curl, ft_limit_rate_curl, url, urlApi)
        y = ft_input(proc);
        if (y == -1): break;
        elif (y == -2): i + 1; continue;
        ##♡♡♡♡♡♡♡♡♡♡♡ Slow curl ♡♡♡♡♡♡♡♡♡♡♡ ##
        ft_limit_rate_curl(url + "cgi-bin/slow_cgi.py", 1);
    ##♡♡♡♡♡♡♡♡♡♡♡ SLEEP AND TEST TO SIEGE♡♡♡♡♡♡♡♡♡♡♡ ##
        ft_for_func(100, ft_real_time_curl, ft_real_time_curl, url, urlApi)
        y = ft_input(proc);
        if (y == -1): break;
        elif (y == -2): i + 1; continue;
    ##♡♡♡♡♡♡♡♡♡♡♡ SLEEP AND TEST TO SIEGE♡♡♡♡♡♡♡♡♡♡♡ ##
    #♡♡♡♡♡♡♡♡♡♡♡CURL SEND BIG FILE ###♡♡♡♡♡♡♡♡♡♡♡    
    ##CREATE A FILE 40mb ##
        ft_post_curl(urlup, fileName);
        ft_post_curl(urlupApi, fileName);
        y = ft_input(proc);
        if (y == -1): break;
        elif (y == -2): i + 1; continue;
    ##♡♡♡♡♡♡♡♡♡♡♡ SLEEP AND TEST TO SIEGE♡♡♡♡♡♡♡♡♡♡♡ ##
    ###♡♡♡♡♡♡♡♡♡♡♡ SLOW_OUTPUT♡♡♡♡♡♡♡♡♡♡♡ ###
    #TODO: RIEMPIRE IL BUFFER DI 1GB VEDER SE SALTA TUTTO ADD WWW/CGI♡♡♡♡♡♡♡♡♡♡♡
        if (config_list[ciclo] != "../../config/TestConf/noBindPort.conf"):
            ft_siege(cmdCGI, MSG_C_T, 1)
            y = ft_input(proc);
            if (y == -1): break;
            elif (y == -2): i + 1; continue;
        ##♡♡♡♡♡♡♡♡♡♡♡ SLEEP AND TEST TO SIEGE♡♡♡♡♡♡♡♡♡♡♡ ##
            y = ft_agrate_test_siege(proc);
            if (y == -1): break;
            elif (y == -2): i + 1; continue;
            time.sleep(2)
    #######♡♡♡♡♡♡♡♡♡♡♡  SUB PROCCESS SIEGE♡♡♡♡♡♡♡♡♡♡♡
        # ♡♡♡♡♡♡♡♡♡♡♡End Valgrind (test più esterno - dopo tutto)♡♡♡♡♡♡♡♡♡♡♡
        print("### FINE VALGRIND ###")
        print(f"==== FINE CICLO {ciclo+1} ====")
        print("Invio Ctrl+C al processo...")
        proc.send_signal(signal.SIGINT)
        time.sleep(0.5)
        ciclo = ft_input_index(ciclo);
        if (ciclo > 0):
            if (ciclo == -1):
                exit();
            elif (ciclo == -2):
                break;

def ft_agrate_test_siege(proc):
        print("### TEST SIEGE ####")
        #print(MSG_L_T)
       # ft_siege(cmdLow, MSG_L_T, 1)
       #print(MSG_M_T)
        #i = ft_input(proc);
        #if (i < 0): return i
        #ft_siege(cmdMini, MSG_M_T, 1);
        #print(MSG_F_T)
        #i = ft_input(proc);
        #if (i < 0): return i
        #ft_siege(cmdFull, MSG_F_T, 1);
        print(MSG_J_T)
        i = ft_input(proc);
        if (i < 0): return i
        ft_siege(cmdBasic, MSG_J_T, 1);
        print(MSG_B_T)
        i = ft_input(proc);
        if (i < 0): return i
        ft_siege(cmdBanch, MSG_B_T, 1);
        i = ft_input(proc);
        if (i < 0): return i


def ft_test_siege(start_index):
    i = start_index;
    while i < len(config_list):
        ft_startGeneratorTxtSiege(i);
        proc = subprocess.Popen(ft_cmdGenerator(i))
        time.sleep(3);
        y = ft_agrate_test_siege(proc);
        if (y == -1): break;
        elif (y == -2): i + 1; continue;
        ft_print_list(config_list)
        i = ft_input_index(i);
        proc.send_signal(signal.SIGINT)
        if (i > 0):
            if (i == -1):
                exit();
            elif (i == -2):
                break;

def ft_test_one_siege(start_index, ft1, cmd, MSG, nbr):
    i = start_index;
    while i < len(config_list):
        ft_startGeneratorTxtSiege(i)
        proc = subprocess.Popen(ft_cmdGenerator(i))
        time.sleep(3);
        ft1(cmd, MSG, 1);
        y = ft_input(proc);
        if (y == -1): break;
        elif (y == -2): i + 1; continue;
        ft_print_list(config_list)
        i = ft_input_index(i);
        proc.send_signal(signal.SIGINT)
        if (i > 0):
            if (i == -1):
                exit();
            elif (i == -2):
                break;

def ft_curl_webserv(flags, start_index):
    i = start_index;
    while i < len(config_list):
        ft_startGeneratorTxtSiege(i);
        proc = subprocess.Popen(ft_cmdGenerator(i))
        time.sleep(3);
        y = ft_input(proc);
        if (y == -1): break;
        elif (y == -2): i + 1; continue;
        if (flags < 0):
            ft_for_func(10, ft_limit_rate_curl, ft_limit_rate_curl, url, urlApi)
        else:
            ft_for_func(100, ft_real_time_curl, ft_real_time_curl, url, urlApi)
        ft_print_list(config_list)
        i = ft_input_index(i);
        proc.send_signal(signal.SIGINT)
        if (i > 0):
            if (i == -1):
                exit();
            elif (i == -2):
                break;

def ft_post_location(start_index, fileToName):
    i = start_index;
    while i < len(config_list):
        ft_startGeneratorTxtSiege(i)
        proc = subprocess.Popen(ft_cmdGenerator(i))
        time.sleep(3);
        y = ft_input(proc);
        if (y == -1): break;
        elif (y == -2): i + 1; continue;
        ft_post_curl(urlup, fileToName)
        ft_post_curl(urlupApi, fileToName)
        ft_print_list(config_list)
        i = ft_input_index(i);
        proc.send_signal(signal.SIGINT)
        if (i > 0):
            if (i == -1):
                exit();
            elif (i == -2):
                break;
  



def main():
    ft_create_big_file()
    ft_startGeneratorTxtSiege(0);
    if os.getenv("FULL") == "1":
        ft_print_list(config_list)
        ft_full_test(ft_input_index(0))
        return ;

    print("♡♡♡ Test Suite ♡♡♡ | Commands: parser-parse | siege-siege | curl-HTTP (soon) | full-full suite (soon) | cgi curl-CGI curl (soon) | cgi siege-CGI Siege (soon) | siege bench-benchmark (soon) | exit-quit | Usage: enter command, choose index or Enter to skip, 'kill' stops process, 'break' stops loop, 'exit' quits | ♡♡♡ Happy testing! ♡♡♡")
    while True:
        inpu = input("\nEnter command: ");
        if inpu == "parser":
            ft_print_list(parser_list)
            ft_parsing(ft_input_index(0))
        elif inpu == "siege":
            ft_print_list(config_list)
            ft_test_siege(ft_input_index(0))
        elif inpu == "curl":
            ft_print_list(config_list)
            ft_curl_webserv(1, ft_input_index(0))
        elif inpu == "curl slow":
            ft_print_list(config_list)
            ft_curl_webserv(-1, ft_input_index(0))
        elif inpu == "full":
            ft_print_list(config_list)
            ft_full_test(ft_input_index(0))
        elif inpu == "curl post":
            ft_print_list(config_list)
            fileToPost = fileName;
            ft_post_location(ft_input_index(0), fileToPost)
       # elif inpu == "curl --resolve":
        #elif inpu == "keep-alive":
        elif inpu == "cgi siege":
            ft_print_list(config_list)
            ft_test_one_siege(ft_input_index(0), ft_siege, cmdCGI, MSG_C_T, 1);
        elif inpu == "siege bench":
            ft_print_list(config_list)
            ft_test_one_siege(ft_input_index(0), ft_siege, cmdBanch, MSG_B_T, 1);
        elif inpu == "siege maxc":
            ft_print_list(config_list)
            ft_test_one_siege(ft_input_index(0), ft_siege, cmdMaxC, MSG_C_T, 1);
        elif inpu == "exit":
            break ;
        elif inpu == "help":
            print("♡♡♡ Test Suite ♡♡♡ | Commands: parser-parse | siege-siege | curl-HTTP (soon) | full-full suite (soon) | cgi curl-CGI curl (soon) | cgi siege-CGI Siege (soon) | siege bench-benchmark (soon) | exit-quit | Usage: enter command, choose index or Enter to skip, 'kill' stops process, 'break' stops loop, 'exit' quits | ♡♡♡ Happy testing! ♡♡♡")
    return ;


if __name__ == "__main__":
    main()

