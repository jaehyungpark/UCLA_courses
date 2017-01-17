.SILENT:
# build an executable file named lab0 from lab0.c
default:
	gcc -o lab0 -g lab0.c

# runs a quick smoke test, using some of the test cases given from the TA
check:
	echo "Hello World" >input.txt

	./lab0 < input.txt >output.txt
	cmp input.txt output.txt || echo "test failed"
	rm -f output.txt

	./lab0 --input=input.txt >output.txt
	cmp input.txt output.txt || echo "test failed"
	rm -f output.txt

	./lab0 --output=output.txt < input.txt
	cmp input.txt output.txt || echo "test failed"
	rm -f output.txt

	./lab0 --input=input.txt --output=output.txt
	cmp input.txt output.txt || echo "test failed"
	rm -f output.txt input.txt
	echo "test passed"	

# delete all makefile-created files to return the directory in initial state
clean:
	rm -f *.o
	rm -f lab0
	rm -f *.txt

# build the distribution tarball
dist:
	rm -f lab0-504212821.tar.gz
	tar -czf lab0-504212821.tar.gz README *.png lab0.c Makefile
