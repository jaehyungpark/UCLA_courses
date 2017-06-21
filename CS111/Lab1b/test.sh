#! /bin/bash

./simpsh --creat --rdonly file1.txt
./simpsh --creat --trunc --nonblock --rdwr file2.txt

./simpsh --verbose --creat --append --trunc --dsync --nofollow --nonblock --cloexec --dictionary \
--sync - excl - trunc --wronly file3.txt > file4.txt

echo "heeello" > file5.txt
echo "wooorld" > file6.txt
touch test.txt
./simpsh -rdonly file5.txt --wronly file6.txt --wronly test.txt --command 0 1 2 cat

echo "hello" > file7.txt
./simpsh --rdonly file7.txt --pipe --creat -- trunc

./simpsh --rdonly file1.txt --wronly file2.txt --wronly file3.txt --wronly file4.txt --close 1 \
--close 3 -- close 0 --close 2