#!/bin/bash


sort_alg=qsort sort=index make -B main.exe
if [ -f time_qn.tsv ]; then
    rm time_qn.tsv
fi
touch time_qn.tsv
for i in {1..500}; do
    echo -e "10*$i" | bc -q | tr -d "\n\r" >> time_qn.tsv
    echo -ne "\t" >> time_qn.tsv
    ./main.exe save $(echo "10*$i" | bc -q) | grep "Sort ticks:" | awk '{print $3}' >> time_qn.tsv
done


sort_alg=qsort sort=data make -B main.exe
if [ -f time_qd.tsv ]; then
    rm time_qd.tsv
fi
touch time_qd.tsv
for i in {1..500}; do
    echo -e "10*$i" | bc -q | tr -d "\n\r" >> time_qd.tsv
    echo -ne "\t" >> time_qd.tsv
    ./main.exe save $(echo "10*$i" | bc -q) | grep "Sort ticks:" | awk '{print $3}' >> time_qd.tsv
done


sort_alg=bubble sort=index make -B main.exe
if [ -f time_bn.tsv ]; then
    rm time_bn.tsv
fi
touch time_bn.tsv
for i in {1..500}; do
    echo -e "10*$i" | bc -q | tr -d "\n\r" >> time_bn.tsv
    echo -ne "\t" >> time_bn.tsv
    ./main.exe save $(echo "10*$i" | bc -q) | grep "Sort ticks:" | awk '{print $3}' >> time_bn.tsv
done


sort_alg=bubble sort=data make -B main.exe
if [ -f time_bd.tsv ]; then
    rm time_bd.tsv
fi
touch time_bd.tsv
for i in {1..500}; do
    echo -e "10*$i" | bc -q | tr -d "\n\r" >> time_bd.tsv
    echo -ne "\t" >> time_bd.tsv
    ./main.exe save $(echo "10*$i" | bc -q) | grep "Sort ticks:" | awk '{print $3}' >> time_bd.tsv
done
