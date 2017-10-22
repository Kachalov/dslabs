#!/bin/bash

for i in $(find . -name 'in_*.txt'); do
    ./main.exe < $i > last_test.txt 2>&1
    diff -su $(echo $i | sed 's/in/out/g') last_test.txt
done

if [ -f last_test.txt ]; then
    rm last_test.txt
fi
