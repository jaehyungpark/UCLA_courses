// Problem 2.73-redo
/* Addition that satursates to TMin or TMax */
/* Minimum and maximum values a 'signed int' can hold. */

#include <limits.h>
#include <stdio.h>
#include <stdbool.h>

int saturating_add(int x, int y){
    int sum = 0;
    int w = (sizeof(int) << 3) -1;
    int checkOF = __builtin_add_overflow(x, y, &sum);
    int signSum = (sum >> w);
    checkOF && (((signSum && (sum = INT_MAX)) || 
                (~signSum && (sum = INT_MIN))));
    return sum;
}
