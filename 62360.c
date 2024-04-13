containsForbiddenChars(const unsigned char * p, int len)
{
	while(len > 0) {
		if(*p < ' ' || *p >= '\x7f')
			return 1;
		p++;
		len--;
	}
	return 0;
}
