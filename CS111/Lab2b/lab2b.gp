#! /usr/bin/gnuplot
#
# purpose:
#	 generate data reduction graphs for multi thread lists
#
# input: lab2_add.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # add operations
#	5. run time (ns)
#	6. run time per operation (ns)
#	7. total sum at end of run (should be zero)
# input: lab2_list.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. run time per operation (ns)
#
# output:
#	lab2_1.png
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.
#

# general plot parameters
set terminal png
set datafile separator ','

set title 'Scalability-1: Synchronized throughput'
set xlabel 'Threads'
set logscale x 2
set xrange [0.75:]
set ylabel 'Throughput (total operations per sec)'
set logscale y 10
set output 'lab2b_1.png'
set key left top
plot \
     "< grep add-m lab2_add.csv" using ($2):(1000000000)/($6) \
	title 'adds w/mutex' with linespoints lc rgb 'green', \
     "< grep add-s lab2_add.csv" using ($2):(1000000000)/($6) \
	title 'adds w/spin' with linespoints lc rgb 'blue', \
     "< grep list-none-m lab2_list.csv" using ($2):(1000000000)/($7) \
	title 'list ins/lookup/delete w/mutex' with linespoints lc rgb 'orange', \
     "< grep list-none-s lab2_list.csv" using ($2):(1000000000)/($7) \
	title 'list ins/lookup/delete w/spin' with linespoints lc rgb 'violet'

set title 'Scalability-2: Per-operation times for list operations'
set xlabel 'Threads'
set logscale x 2
set xrange [0.75:]
set ylabel 'mean time per operation (ns)'
set logscale y 10
set output 'lab2b_2.png'
set key left top
plot \
     "< grep -E 'list-none-m,(1|2|4|8|16|24),1000,1,' lab2b_list.csv" using ($2):($7) \
	title 'completion time' with linespoints lc rgb 'orange', \
     "< grep -E 'list-none-m,(1|2|4|8|16|24),1000,1,' lab2b_list.csv" using ($2):($8) \
	title 'wait for lock' with linespoints lc rgb 'red'

set title 'Scalability-3: Correct synchronization of partitioned lists'
set xlabel 'Threads'
set logscale x 2
set xrange [0.75:]
set ylabel 'Successful iterations'
set logscale y 10
set output 'lab2b_3.png'
set key left top
plot \
     "< grep list-id-none lab2b_list.csv" using ($2):($3) \
	title 'yield=id' with points lc rgb 'red', \
     "< grep list-id-s lab2b_list.csv" using ($2):($3) \
	title 'Mutex' with points lc rgb 'green', \
     "< grep list-id-m lab2b_list.csv" using ($2):($3) \
	title 'Spin-Lock' with points lc rgb 'blue'

set title 'Scalability-4: Mutex synchronization throughput of partitioned lists'
set xlabel 'Threads'
set logscale x 2
set xrange [0.75:]
set ylabel 'Throughput (total operations per sec)'
set logscale y 10
set output 'lab2b_4.png'
set key left top
plot \
     "< grep -E 'list-none-m,(1|2|4|8|12),1000,1,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title 'lists=1' with linespoints lc rgb 'violet', \
     "< grep -E 'list-none-m,(1|2|4|8|12),1000,4,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title 'lists=4' with linespoints lc rgb 'green', \
     "< grep -E 'list-none-m,(1|2|4|8|12),1000,8,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title 'lists=8' with linespoints lc rgb 'blue', \
     "< grep -E 'list-none-m,(1|2|4|8|12),1000,16,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title 'lists=16' with linespoints lc rgb 'orange'

set title 'Scalability-5: Spin lock synchronization throughput of partitioned lists'
set xlabel 'Threads'
set logscale x 2
set xrange [0.75:]
set ylabel 'Throughput (total operations per sec)'
set logscale y 10
set output 'lab2b_5.png'
set key left top
plot \
     "< grep -E 'list-none-s,(1|2|4|8|12),1000,1,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title 'lists=1' with linespoints lc rgb 'violet', \
     "< grep -E 'list-none-s,(1|2|4|8|12),1000,4,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title 'lists=4' with linespoints lc rgb 'green', \
     "< grep -E 'list-none-s,(1|2|4|8|12),1000,8,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title 'lists=8' with linespoints lc rgb 'blue', \
     "< grep -E 'list-none-s,(1|2|4|8|12),1000,16,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title 'lists=16' with linespoints lc rgb 'orange'
