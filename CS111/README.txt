CS 111 Lab3a
Jaehyung Park
504-212-821

the submitted tar.gz contains 3 files
- lab3a.c, README.txt, Makefile

lab3a.c was tested on lnxsrv09.seas.ucla.edu

Things to mention:
- initial testing was done on a native Linux machine mounting the given disk_image file
- in Makefile, used diff to check from the solution *.csv files and compared them with the results I produced
  all the other .csv files were okay, except for inode.csv. Comparing the results, there were total
  7 entires that were not identical.
- because of this, running make check stops after diff inode. need to manual check for directory.csv and indirect.csv
- make clean clears all csv and txt files
