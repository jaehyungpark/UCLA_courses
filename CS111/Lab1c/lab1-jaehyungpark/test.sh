#! /bin/bash

# create random files
base64 /dev/zero | head -c 5000000 > zeros.txt
base64 /dev/urandom | head -c 5000000 > randoms.txt
base64 /dev/urandom | head -c 5000000 > randoms2.txt
# benchmark to filter medium sized files(50MB), running random and zero filled files

echo > sort.txt
echo > randomsort.txt
echo > error.txt
echo > zerosort.txt

echo Benchmark 1
./simpsh --profile --wait --rdonly zeros.txt --wronly sort.txt --wronly error.txt --command 0 1 2 sort
./simpsh --profile --wait --rdonly randoms.txt --wronly randomsort.txt --wronly error.txt --command 0 1 2 sort

echo Benchmark 2
./simpsh --profile --wait --rdonly zeros.txt --pipe --pipe --wronly zerosort.txt --wronly error.txt --command 3 5 6 cat - --command 0 2 6 sort --command 1 4 6 tr 'A-Z' 'a-z'
./simpsh --profile --wait --rdonly randoms.txt --pipe --pipe --wronly randomsort.txt --wronly error.txt --command 3 5 6 cat - --command 0 2 6 sort --command 1 4 6 tr 'A-Z' 'a-z'

echo Benchmark 3
./simpsh --profile --wait --rdonly zeros.txt --wronly zerosort.txt --wronly error.txt --command 0 1 2 uniq -u
./simpsh --profile --wait --rdonly randoms.txt --wronly randomsort.txt --wronly error.txt --command 0 1 2 uniq -u 

echo Benchmark 4
./simpsh --profile --wait --rdonly randoms.txt --wronly randomsort.txt --wronly error.txt --command 0 1 2 sed 's/a*b//g'

