touch test1.txt
touch test2.txt
touch test3.txt
./simpsh --rdonly test1.txt --wronly test2.txt --wronly test3.txt --command 0 1 2 ls
./simpsh --verbose --rdonly test1.txt --wronly test2.txt --wronly test3.txt --command 0 1 2 cat test1.txt
./simpsh --rdonly test1.txt --wronly test2.txt --wronly test3.txt --verbose --command 0 1 2 cat
./simpsh --rdonly test1.txt --wronly test2.txt --wronly test3.txt --command 0 1 2 cat --verbose
