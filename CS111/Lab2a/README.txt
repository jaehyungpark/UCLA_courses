CS 111 Lab 2a
Jaehyung Park
504212821

README.txt
This file contains answers for each of the questions listed in the lab website

Other files/contents for the lab submission includes:
lab2_list.c, lab2_add.c, SortedList.c, SortedList.h for source files
*.csv files and *.png files for data and graphs

All tests have been ran on lnxsrv09.seas.ucla.edu.


QUESTION 2.1.1 - causing conflicts:
Why does it take many iterations before errors are seen?
  - Taking many interations will increase the total amount of accesses to the counter,
    which the possibility of more than two or three threads will try to access at the
    same time simultaneously.
Why does a significantly smaller number of iterations so seldom fail?
  - Compared to the overall runtime of executing program, updating smaller number of
    iterations is faster than the overall runtime, which the program seldom fails to 
    update it.


QUESTION 2.1.2 - cost of yielding:
Why are the --yield runs so much slower?
  - Yield option causes a context switch for each interation of calling add().
Where is the additional time going?
  - This additional time is going/used creating context switches.
Is it possible to get valid per-operation timings if we are using the --yield option?  
If so, explain how.  If not, explain why not.
  - To get a valid per-operation timing with --yield option will be difficult since
    most of the time, timing takes a lot of time as it performs context switching.


QUESTION 2.1.3 - measurement errors:
Why does the average cost per operation drop with increasing iterations?
  - As iteration increases, the overhead of creating threads and joining them becomes
    less significant, making the average cost per operation to drop.
If the cost per iteration is a function of the number of iterations, how do we know 
how many iterations to run (or what the “correct” cost is)?
  - One possible way could be make all the threads are created as the timer starts running.


QUESTION 2.1.4 - costs of serialization:
Why do all of the options perform similarly for low numbers of threads?
  - With smaller number of threads, there is not much memory contention that can cause
    differences in performance using different options, which leads to similar performance
    running in lower number of threads.
Why do the three protected operations slow down as the number of threads rises?
  - There are more memory contention with more than 1 threads that each threads will spend
    extra time of waiting for the lock.
Why are spin-locks so expensive for large numbers of threads?
  - Comparing the cost of spin-locks to mutexes, spin-locks take more CPU cycles as it is
    more expensive to use than mutexes, which makes it harder for a running thread to
    lock it.


QUESTION 2.2.1 - scalability of Mutex
Compare the variation in time per protected operation vs the number of threads 
(for mutex-protected operations) in Part-1 and Part-2, commenting on 
similarities/differences and offering explanations for them.
  - The timing order for using linked list increases as it iterates through the list
    due to its complex mechanism, however, both part 1 and part 2 have an increasing
    pattern related to the memory contention with more than 1 threads (many threads) as
    both of them takes longer time. The differences will be for part 1, cost per
    operation using mutexes seems to increase is a faster pace after increasing the number 
    of threads (in this case after 4 threads). One of the reasons for this is the
    threshold or maximum number of threads could be around 4 as we take more than 4 threads,
    the machine can't handle it all at once.


QUESTION 2.2.2 - scalability of spin locks
Compare the variation in time per protected operation vs the number of threads 
for Mutex vs Spin locks, commenting on similarities/differences and offering explanations for them.
  - Looking at the time per protected operation vs the number of threads for Mutex vs Spin locks,
    the results show similar increasing patterns and values for both of them in part 2.
    One of the differences in part 2 from part 1 is when using linked list, the locking
    takes more time as locking and unlocking with linked list doesn't happend that much
    compared to as in part 1.