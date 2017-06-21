# Make file
.SILENT:
default:
	gcc -o simpsh simpsh.c

check:
	chmod +x test.sh
	./test.sh

dist:
	tar -czf lab1-jaehyungpark.tar.gz README simpsh.c Makefile test.sh

clean:
	rm -f lab1-jaehyungpark.tar.gz 
	rm -f *.o 
	rm -f *.txt 
	rm -f simpsh
