// Problem 2.73
/* Addition that satursates to TMin or TMax */
/* Minimum and maximum values a 'signed int' can hold. */

#include <limits.h>
#include <stdio.h>

int saturating_add(int x, int y){
    int sum = x + y;
    int w = (sizeof(int) << 3) -1;
    int signX = (x >> w);
    int signY = (y >> w);
    int signSum = (sum >> w);
    int posOverflow = (~(signX | signY)) & signSum;
    int negOverflow = (signX & signY) & ~signSum;
    int getResult = (~(posOverflow | negOverflow) & sum) |
        (posOverflow & INT_MAX) | (negOverflow & INT_MIN);
    return getResult;
}
