#!/bin/bash
lscpu | grep "Model name" > result.txt
lscpu | grep "CPU(s):" | grep -v "NUMA" >> result.txt
lscpu | grep "cach" >> result.txt
echo "Hostname:		$(hostname)" >> result.txt
echo File system: >> result.txt
df -T | tr -s ' ' | cut -f 2 -d ' ' | grep -v "Type\|/" >> result.txt
