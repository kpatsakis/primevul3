_tiffWriteProc(thandle_t fd, void* buf, tmsize_t size)
{
	/* tmsize_t is 64bit on 64bit systems, but the WinAPI WriteFile takes
	 * 32bit sizes, so we loop through the data in suitable 32bit sized
	 * chunks */
	uint8* ma;
	uint64 mb;
	DWORD n;
	DWORD o;
	tmsize_t p;
	ma=(uint8*)buf;
	mb=size;
	p=0;
	while (mb>0)
	{
		n=0x80000000UL;
		if ((uint64)n>mb)
			n=(DWORD)mb;
		if (!WriteFile(fd,(LPVOID)ma,n,&o,NULL))
			return(0);
		ma+=o;
		mb-=o;
		p+=o;
		if (o!=n)
			break;
	}
	return(p);
}
