# CS 111 lab 2b
# Jaehyung park
# 504212821

	SOURCE_FILES1 = lab2_list.c SortedList.c SortedList.h
	GRAPH_FILES1 = lab2b_1.png lab2b_2.png lab2b_3.png lab2b_4.png lab2b_5.png
	PROFILE_FILES1 = profile.gperf
	OTHER_FILES1 = lab2b_list.csv lab2_add.csv lab2_list.csv

build:
	gcc -pthread $(SOURCE_FILES1) -o lab2_list

profile:
	gcc -g -pthread $(SOURCE_FILES1) -o lab2_list
	LD_PRELOAD=/u/ee/ugrad/jaehyung/lib/libprofiler.so CPUPROFILE=./raw.gperf ./lab2_list --threads=12 --iterations=1000 --sync=s
	pprof --text ./lab2_list ./raw.gperf > profile.gperf
	pprof --list=SortedList_insert_spin ./lab2_list ./raw.gperf >> profile.gperf
	rm raw.gperf

graphs:
	gnuplot lab2b.gp

tarball:
	tar -czvf lab2b-504212821.tar.gz $(SOURCE_FILES1) $(GRAPH_FILES1) $(PROFILE_FILES1) $(OTHER_FILES1) README.txt Makefile lab2b.gp

clean:
	rm lab2_list $(GRAPH_FILES1) $(PROFILE_FILES1) *.tar.gz

tests:
	rm -f lab2b_list.csv
	
	./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=1 --yield=id >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=2 --yield=id >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=4 --yield=id >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=8 --yield=id >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=16 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=4 --iterations=1 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=4 --iterations=2 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=4 --iterations=4 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=4 --iterations=8 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=4 --iterations=16 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=8 --iterations=1 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=8 --iterations=2 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=8 --iterations=4 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=8 --iterations=8 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=8 --iterations=16 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=12 --iterations=1 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=12 --iterations=2 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=12 --iterations=4 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=12 --iterations=8 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=12 --iterations=16 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=16 --iterations=1 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=16 --iterations=2 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=16 --iterations=4 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=16 --iterations=8 --yield=id >> lab2b_list.csv
	-./lab2_list --lists=4 --threads=16 --iterations=16 --yield=id >> lab2b_list.csv	
	./lab2_list --lists=4 --threads=1 --iterations=10 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=20 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=40 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=80 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=4 --iterations=10 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=4 --iterations=20 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=4 --iterations=40 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=4 --iterations=80 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=8 --iterations=10 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=8 --iterations=20 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=8 --iterations=40 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=8 --iterations=80 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=12 --iterations=10 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=12 --iterations=20 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=12 --iterations=40 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=12 --iterations=80 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=16 --iterations=10 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=16 --iterations=20 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=16 --iterations=40 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=16 --iterations=80 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=10 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=20 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=40 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=80 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=4 --iterations=10 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=4 --iterations=20 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=4 --iterations=40 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=4 --iterations=80 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=8 --iterations=10 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=8 --iterations=20 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=8 --iterations=40 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=8 --iterations=80 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=12 --iterations=10 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=12 --iterations=20 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=12 --iterations=40 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=12 --iterations=80 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=16 --iterations=10 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=16 --iterations=20 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=16 --iterations=40 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=16 --iterations=80 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --lists=1 --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=1 --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=1 --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=1 --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=1 --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=8 --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=8 --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=8 --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=8 --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=8 --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=16 --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=16 --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=16 --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=16 --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=16 --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=1 --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=1 --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=1 --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=1 --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=1 --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=8 --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=8 --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=8 --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=8 --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=8 --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=16 --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=16 --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=16 --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=16 --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=16 --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
