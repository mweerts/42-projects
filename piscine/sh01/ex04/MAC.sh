#!/bin/bash
ifconfig | grep "ether " | sed "s/ether //" | tr -d '\t '