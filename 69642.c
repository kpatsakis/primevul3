dtoieee(double *dp)
{
	double_t num;
	double x;
	int exp;

	num.d = *dp;
	if (!num.d) {			/* Zero is just binary all zeros */
		num.l[0] = num.l[1] = 0;
		return;
	}

	if (num.d < 0) {		/* Sign is encoded separately */
		num.d = -num.d;
		num.ieee.sign = 1;
	} else {
		num.ieee.sign = 0;
	}

	/* Now separate the absolute value into mantissa and exponent */
	x = frexp(num.d, &exp);

	/*
	 * Handle cases where the value is outside the
	 * range for IEEE floating point numbers. 
	 * (Overflow cannot happen on a VAX, but underflow
	 * can happen for G float.)
	 */
	if (exp < -1022) {		/* Unnormalized number */
		x = ldexp(x, -1023-exp);
		exp = 0;
	} else if (exp > 1023) {	/* +/- infinity */
		x = 0;
		exp = 2047;
	} else {			/* Get rid of most significant bit */
		x *= 2;
		x -= 1;
		exp += 1022; /* fix NDR: 1.0 -> x=0.5, exp=1 -> ieee.exp = 1023 */
	}
	num.ieee.exp = exp;

	x *= (double) (1<<20);
	num.ieee.mant = (long) x;
	x -= (double) num.ieee.mant;
	num.ieee.mant2 = (long) (x*((double) (1<<16)*(double) (1<<16)));

	if (!(num.ieee.mant || num.ieee.exp || num.ieee.mant2)) {
		/* Avoid negative zero */
		num.ieee.sign = 0;
	}
	((double_t*)dp)->ieee = num.ieee;
}
