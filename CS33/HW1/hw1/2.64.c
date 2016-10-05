// Problem 2.64
/* Return 1 when any odd bit of x equals 1; 0 otherwise. Assume w=32 */
// assuming the data type int has w = 32 bits, we can 

int any_odd_one(unsigned x){
    return (x & 0xAAAAAAAA) != 0;
}
