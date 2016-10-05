The homework was to enable the given code to render the image in different 
(multi) threads and see the performace, starting from 1, 2, 4, and 8 threads.

In the specs, we were told to modify the code only in main.c and Makefile,
so I looked at main.c and found the code is available for single thread 
only. Any threads other than one would give an error.

I found the nested loops in main.c that took arguments of width and height
need to be modified to process more than 1 threads, so I created a function 
by pulling this whole code out to a function named multi_thread to separate it
from the main function.

Some issues I ran into:
At first, I had an idea to split the image into 2 sides for each x and y
coordinates. By this, I can assign each thread to process the 1/4th of the
image. Then I realized there will be rounding errors for odd number of
pixels. For instance, the image is 131 by 131 pixels, if I divide it in 2,
then 0~65 and 66~131 will be splitted, since 131/2 = 65.5 is rounded down to 
65. This leaves a gap between 65 and 66.

(Brief) Approach:
Instead of using 2D arrays with width and height, I added another dimension
by adding color as the third that is assigned from scaled_color. This added 
line stores each width, height, and color to print in the main function and 
render the image. Also, the void * multi_thread function is passed by width
as px and height as py and equally processed in every 1 pixel by incrementing 
px argument by nthreads.

After changing main.c, I changed Makefile to test 1,2,4,8 threads.

Overall results:
After modifying the codes, below are the results:

1 thread:
read 0m41.393s
user 0m41.385s
sys  0m0.001s

2 threads:
read 0m20.963s
user 0m41.445s
sys  0m001s

4 threads:
read 0m11.227s
user 0m42.958s
sys  0m0.003s

8 threads:
read 0m6.311s
user 0m48.045s
sys  0m0.003s

Looking at the read times, it was interesting to see that for all 4 cases, 
(# of threads) * (read time) converges to a similar figure between 41 to 49.
This shows that 8 threads is almost 6 times faster than 1 thread for doing
the same work. This also gave me the impression that rendering is completely
CPU dependent that the more threads we have the more we can save time.
