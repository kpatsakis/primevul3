static long jas_iccpadtomult(long x, long y)
{
	return ((x + y - 1) / y) * y;
}
