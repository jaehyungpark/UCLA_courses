CS 111 Lab 2b
Jaehyung Park
504212821

README.txt
This file contains answers for each of the questions listed in the lab website

Other files/contents for the lab submission includes:
lab2_list.c, SortedList.c, SortedList.h for source files
lab2b_1.png lab2b_2.png lab2b_3.png lab2b_4.png lab2b_5.png for graph files
profile.gperf for profile file after running make profile
lab2b_list.csv is the file generated after running make test
lab2_add.csv lab2_list.csv are files generated from previous the lab
lab2b.gp is the graphing script for creating *.png files
*.csv files and *.png files for data and graphs

All tests have been ran on lnxsrv09.seas.ucla.edu.



QUESTION 2.3.1 - Cycles in the basic implementation:
Where do you believe most of the cycles are spent in the 1 and 2-thread tests (for both add and list)?  
Why do you believe these to be the most expensive parts of the code?
  - Memory contention is that a big issue when running with few (1 and 2) threads, which most of the running time is spent on running 
    operations such as add and list operations.
Where do you believe most of the time/cycles are being spent in the high-thread spin-lock tests?
  - Running with higher number of threads, spin locks for example, took time spinning while more number of threads used more memory 
    contention and required more time to acquire locks. For spin lock tests, most of the time are spent waiting.
Where do you believe most of the time/cycles are being spent in the high-thread mutex tests?
  - Most of the time/cycles would be spent on context switching, which running with higher/more number of threads leads to more memory 
    contention and requires longer wait time to acquire locks. Mutex lock for example, threads will not be able to acquire a lock when 
    it's being yield, which causes an expensive performance issue for contex switching.


QUESTION 2.3.2 - Execution Profiling:
Where (what lines of code) are consuming most of the cycles when the spin-lock version of the list exerciser is run with a large number of threads?
  - Looking at the profile.gperf file, it shows that SortedList_insert_spin for running spin lock takes most of the cycle, which is pretty obvious 
    to observe as lab2_list is not able to acquire a lock and just continuously spins by going over the condition.
Why does this operation become so expensive with large numbers of threads?
  - Larger number of threads comes with problems for each single thread to process the list at once, while that single thread is using the list, all
    of ther other threads check back continuously on the lock, which the remaining threads will keep spinning and use the CPU resources/cycles until
    the thread is able to acquire the lock. Since the lock is exclusive, as the number of threads increase, it required more cost for the cpu to check.


QUESTION 2.3.3 - Mutex Wait Time:
Look at the average time per operation (vs # threads) and the average wait-for-mutex time (vs #threads).  
Why does the average lock-wait time rise so dramatically with the number of contending threads?
  - Each list can be operated by one thread (same for lock), because of this and average lock-wait time will rise as the number of contending thread 
    increases. Since it requires more time for larger number of threads, the lock wait time also increases which leads to rise the average lock-wait time.
Why does the completion time per operation rise (less dramatically) with the number of contending threads?
  - The CPU still executes operations all the time while other threads are still waiting, so increasing the number of threads will affect the
    CPU's processing to decrease the operation speed while the ownership of the lock changes to another thread. (Time spent for waiting a lock
    is included to the operation time to complete the lock as it increases with the number of contending threads)
How is it possible for the wait time per operation to go up faster (or higher) than the completion time per operation?
  - The completion time per operation gradually increases as each thread is being added, which the time will increase due to thread switching.
    On the other hand, the wait time per operation rises in a faster rate that the completion time per operation as each thread doesn't own
    a lock. This requires an extra cycle to acquiring a lock and adds more time for the extra thread, which the wait time per operation
    goes up faster than the completion time per operation.


QUESTION 2.3.4 - Performance of Partitioned Lists
Explain the change in performance of the synchronized methods as a function of the number of lists.
  - As we increase the list, the throughput is also increased for synchronized methods. Overall, we could observe that shynchronized testings
    gave us more efficient results while increasing the list as with the number of threads. For each list, there is a sepearate lock and we
    allocate a portion of memory space that can be changed, which makes it possible to reduce memory contention and times spent for each thread
    waiting on locks. (eventually, improving the overall performance) 
Should the throughput continue increasing as the number of lists is further increased?  If not, explain why not.
  - Since machines can only handle a limited amount of threads, increasing the number of lists after the limist will not improve the performance.
It seems reasonable to suggest the throughput of an N-way partitioned list should be equivalent to 
the throughput of a single list with fewer (1/N) threads.  Does this appear to be true in the above curves?  If not, explain why not.
  - The assumption seems to be reasonable mathematically when we only consider for single thread performance by dividing N way lists with N (1/N threads), 
    but looking at the processing power of the CPU with many cores these days, the throughput is higher when running on multithreads and many lists than 
    of 1 thread and list each. This could be one of the advantages of parallel computing where memory contention gets smaller (more access to shared
    memories and lists per thread) and the fact that the program's critical section can be accessed by more than 1 thread leads to increase the throughput
    with more threads.
