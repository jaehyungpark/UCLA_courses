# CS 111 lab3b
# Jaehyung Park
# 504-212-821

SOURCE_FILES = lab3b.py
OTHER_FILES = Makefile README.txt

run:
	python lab3b.py >lab3b_check.txt

clean:
	rm lab3b_check.txt

dist:
	tar -cvzf lab3b-504212821.tar.gz $(SOURCE_FILES) $(OTHER_FILES)

# make check for testing purposes
# had solution lab3b_check.txt file under solution directory
check:
	# we sort both lab3b_check.txt files and use diff to compare
	python lab3b.py >lab3b_check.txt
	sort lab3b_check.txt >lab3b_check_sorted.txt
	sort solution/lab3b_check.txt >solution/lab3b_check_sorted.txt
	diff lab3b_check_sorted.txt solution/lab3b_check_sorted.txt
	rm lab3b_check_sorted.txt solution/lab3b_check_sorted.txt
