# Make file
.SILENT:
default:
	gcc -o simpsh simpsh.c

check:
	chmod +x test.sh
	./test.sh

dist:
	tar -czvf lab1-jaehyungpark.tar.gz README simpsh.c Makefile test.sh bash_benchmark.sh simpsh_benchmark.sh

clean:
	rm -f lab1-jaehyungpark.tar.gz 
	rm -f *.o 
	rm -f *.txt 
	rm -f simpsh
	rm -rf lab1cread*
