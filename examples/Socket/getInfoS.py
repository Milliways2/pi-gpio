#! /usr/bin/env python3
"""
This program displays Raspberry Pi Information.
"""
# 2023-09-08

from pi_sock import *

host = "localhost";
if len(sys.argv) > 1:
  host = sys.argv[1];   # Optional host address

socket_connect(host)

# InfoDict = eval(get_rpi_info())  # string return needs to be evaluated to dictionary
InfoDict = get_rpi_info()

for k in InfoDict:
    print(k, ':\t', InfoDict[k])

