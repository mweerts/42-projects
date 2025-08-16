#!/usr/bin/env python3

import time
print("Content-Type: text/plain\n")
for i in range(100):
    print(f"Line {i}")
    time.sleep(0.1)
