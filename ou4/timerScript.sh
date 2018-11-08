#!/bin/sh



echo "Hello world"

THREAD_MAX=8
ITER=10

for ((i = 1; i <= THREAD_MAX; i++))
do
    for ((j = 0; j < ITER; j++))
    do
        time[$j]=`./mfind -p 5 . Makefile\* 2>&1 >/dev/null`
        echo ${time[*]}
    done
    echo ${time[*]} > data.log
done
