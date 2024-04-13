void printversion(const unsigned char * resp, int n)
{
	int l;
	const unsigned char * p;

	p = resp;
	DECODELENGTH(l, p);
	if(resp + n < p + l) {
		printf("get version error\n");
	}
	printf("MiniSSDPd version : %.*s\n", l, p);
}
