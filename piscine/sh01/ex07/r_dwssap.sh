#!/bin/bash
awk -F ":" '!/^#/ && NR%2==0{print $1}' /etc/passwd | rev | sort -r | sed -n $FT_LINE1','$FT_LINE2'p' | awk '{printf "%s%s",(NR>1?", ":""),$NF;} END{print"."}' | tr -d '\n'