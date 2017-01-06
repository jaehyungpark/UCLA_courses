// Problem 3.60 fill in the missing parts of the C code

long loop(long x, long n)
{
	long result = 0;
	long mask;
	for (mask = 1; mask != 0; mask = mask << (n & 0xFF)){
		result |= (x & mask);
	}
	return result;
}
