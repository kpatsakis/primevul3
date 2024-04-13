ieeetod(double *dp)
{
	double_t source;
	long sign,exp,mant;
	double dmant;

	source.ieee = ((double_t*)dp)->ieee;
	sign = source.ieee.sign;
	exp = source.ieee.exp;
	mant = source.ieee.mant;

	if (exp == 2047) {
		if (mant)			/* Not a Number (NAN) */
			*dp = HUGE_VAL;
		else				/* +/- infinity */
			*dp = (sign ? -HUGE_VAL : HUGE_VAL);
		return;
	}
	if (!exp) {
		if (!(mant || source.ieee.mant2)) {	/* zero */
			*dp=0;
			return;
		} else {			/* Unnormalized number */
			/* NB: not -1023, the 1 bit is not implied */
			exp= -1022;
		}
	} else {
		mant |= 1<<20;
		exp -= 1023;
	}
	dmant = (((double) mant) +
		((double) source.ieee.mant2) / (((double) (1<<16)) *
		((double) (1<<16)))) / (double) (1<<20);
	dmant = ldexp(dmant, exp);
	if (sign)
		dmant= -dmant;
	*dp = dmant;
}
