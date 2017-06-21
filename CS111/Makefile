# CS 111 Lab 3a
# Jaehyung Park
# 504-212-821

TARGET_FILE = lab3a
OTHER_FILES = Makefile README.txt

build:
	gcc $(TARGET_FILE).c -o $(TARGET_FILE)

dist:
	tar -cvzf lab3a-504212821.tar.gz $(TARGET_FILE).c $(OTHER_FILES)

clean:
	rm *.csv out* lab3a

# below check is added test outputs and easily compare the solution files
check:
	# created output.txt files to check the diff results
	./lab3a disk-image
	diff super.csv solutions/super.csv >> out1.txt
	diff group.csv solutions/group.csv >> out2.txt
	diff bitmap.csv solutions/bitmap.csv >> out3.txt
	diff inode.csv solutions/inode.csv >> out4.txt
	diff directory.csv solutions/directory.csv >> out5.txt
	diff indirect.csv solutions/indirect.csv >> out6.txt
