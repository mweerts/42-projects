#!/usr/bin/env python3
import time
import sys

# Intestazioni CGI
print("Content-Type: text/plain\n")

# Simuliamo output lento e abbondante
for i in range(1, 501):  # 500 righe
    print(f"Linea {i}: Lorem ipsum dolor sit amet, consectetur adipiscing elit.")
    sys.stdout.flush()  # forza l’invio al client
    time.sleep(0.05)    # 50ms di ritardo per simulare lentezza

