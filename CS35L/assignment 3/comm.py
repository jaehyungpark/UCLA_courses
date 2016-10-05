#!/usr/bin/python

## script comm.py in the style of randline.py
import random, sys
from optparse import OptionParser

class comm:
    def __init__(self, filename):
        f = open(filename, 'r')
        self.lines = f.readlines()
        f.close()

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [-1 -2 -3 -u]... FILE1 FILE2
Output at most 3 columns of unique or matching lines between the files"""

    parser = OptionParser(version=version_msg, usage=usage_msg)
    parser.add_option("-1", action="store_true", dest="column1", default=False,
                      help="unique words on file 1")
    parser.add_option("-2", action="store_true", dest="column2", default=False,
                       help="unique words on file 2")
    parser.add_option("-3", action="store_true", dest="column3", default=False,
                      help="common words on both files 1 and 2")
    parser.add_option("-u", action="store_true", dest="unsorted", 
                default=False, help="comm word even if words are not sorted")

    options, args = parser.parse_args(sys.argv[1:])

    # if the input files are not 2, send out error msg
    if len(args) != 2:
        parser.error("wrong number of files")

    file1 = args[0] #first file
    file2 = args[1] #second file
    cols1 = []
    cols2 = []

    # checking files
    if file2 == "-":
        cols1 = sys.stdin.readlines()

        try:
            second = open(file1,'r')
            cols2 = second.readlines()
            second.close()
        except:
            parser.error("wrong file")

    else:
        try:
            first = open(file1,'r')
            cols1 = first.readlines()
            first.close()
            second = open(file2,'r')
            cols2 = second.readlines()
            second.close()
        except:
            parser.error("wrong file")

    a1 = []
    b2 = []
    c3 = []

    # parser options
    unsorted=options.unsorted
    column1=options.column1
    column2=options.column2
    column3=options.column3

    # when -u is not used
    if unsorted == 0:
        c3=[j for j in cols1 if j in cols2]
        for j in c3:
            if j in cols2:
                cols2.remove(j)
        for j in c3:
            if j in cols1:
                cols1.remove(j)
        k = sorted(cols1+cols2+c3)
        a1 = cols1
        b2 = cols2

        for j in k:
            # defining every possible option combination
            if j in a1 and column1 != 1:
                sys.stdout.write(j)
            if j in a1 and column1 != 1 and column2 != 1:
                sys.stdout.write("       "+j)
            elif j in b2 and column1 == 1 and column2 != 1:
                sys.stdout.write(j)

            if j in c3 and column1 != 1 and column2 != 1 and column3 != 1:
                sys.stdout.write("              "+j)
            elif j in c3 and column1 != 1 and column2 == 1 and column3 != 1:
                sys.stdout.write("       "+j)
            elif j in c3 and column1 == 1 and column2 != 1 and column3 != 1:
                sys.stdout.write("       "+j)
            elif j in c3 and column1 == 1 and column2 == 1 and column3 != 1:
                sys.stdout.write(j)

    # when -u is used
    if unsorted == 1:
        c3 = [j for j in cols1 if j in cols2]
        for j in c3:
            if j in cols2:
                cols2.remove(j)
        for j in cols1:
            if j in c3 and column1 != 1 and column2 != 1 and column3 != 1:
                sys.stdout.write("              "+j)
            elif j in c3 and column1 != 1 and column2 == 1 and column3 != 1:
                sys.stdout.write("       "+j)
            elif j in c3 and column1 == 1 and column2 != 1 and column3 != 1:
                sys.stdout.write("       "+j)
            elif j in c3 and column1 == 1 and column2 == 1 and column3 != 1:
                sys.stdout.write(j)
            elif j not in c3 and column1 != 1:
                sys.stdout.write(j)
        for j in cols2:
            if column1 != 1 and column2 != 1:
                sys.stdout.write("       "+j)
            elif column1 == 1 and column2 != 1:
                sys.stdout.write(j)

if __name__ == "__main__":
    main()
