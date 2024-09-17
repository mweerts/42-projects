#!/bin/sh
echo  "ibase=5;$FT_NBR1+$FT_NBR2" | tr "\'\"\?\!\\" "02341" | tr "mrdoc" "01234" | bc | awk 'END{print "obase=13;"$0}' | bc | tr "0123456789ABC" "gtaio luSnemf"