#! /bin/bash
# Running a benchmark on bash 

echo Benchmark 1
time sort zeros.txt > zerosort.txt 2>error.txt
time sort randoms.txt > randomsort.txt 2>error.txt

echo Benchmark 2
time sort zeros.txt | tr 'a-z''A-Z' | cat - > zerosort.txt 2>error.txt
time sort randoms.txt | tr 'a-z''A-Z' | cat - > randomsort.txt 2>error.txt

echo Benchmark 3
time uniq -u zeros.txt > zerosort.txt 2>error.txt
time uniq -u randoms.txt > randomsort.txt 2>error.txt

echo Benchmark 4
time sed 's/a*b//g' randoms.txt > randomsort.txt 2>error.txt 

