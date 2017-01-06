// CS 33 HW 4
// Prob 2.90

// for this c program, we are going to change unsigned to unsigned long
// and float to double.
extern double u2f(unsigned long);

double fpwr2(int x)
{
	/* Result exponent and fraction */
	unsigned long exp, frac;
	unsigned long u;

	if (x < -1074) {
		/* Too small. Return 0.0 */
		exp = 0;
		frac = 0;
	} else if (x < -1022) {
		/* Denormalized result */
		exp = 0;
		frac = 1 << (x + 1074); // 52 - (-x - 1023) - 1
	} else if (x < 1024) {
		/* Normalized result */
		exp = x + 1023;	// 2^10 - 1
		frac = 0;
	} else {
		/* Too big. Return +oo */
		exp = 2047;
		frac = 0;
	}

	/* Pack exp and frac into 64 bits */
	/* Because double has 52 fraction bits */
	u = exp << 52 | frac;
	/* Return as double */
	/* I guess this part is unnecessary */
	return u2f(u);
}
