static int checkColorMap(n, r, g, b)
int n;
uint16 *r, *g, *b;
{
	while (n-- > 0)
		if (*r++ >= 256 || *g++ >= 256 || *b++ >= 256)
			return (16);
	return (8);
}
