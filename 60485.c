strtoint_clipped(const char *const str, int min, int max)
{
	int r = strtoint(str);
	if (r == -1)
		return r;
	else if (r<min)
		return min;
	else if (r>max)
		return max;
	else
		return r;
}
