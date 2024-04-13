TIFFOpen(const char* name, const char* mode)
{
	static const char module[] = "TIFFOpen";
	int m, fd;

	m = _TIFFgetMode(mode, module);
	if (m == -1)
		return ((TIFF*)0);
        if (m&O_TRUNC){
                /*
		 * There is a bug in open in VAXC. If you use
		 * open w/ m=O_RDWR|O_CREAT|O_TRUNC the
		 * wrong thing happens.  On the other hand
		 * creat does the right thing.
                 */
                fd = creat((char *) /* bug in stdio.h */ name, 0666,
		    "alq = 128", "deq = 64", "mbc = 32",
		    "fop = tef");
	} else if (m&O_RDWR) {
		fd = open(name, m, 0666,
		    "deq = 64", "mbc = 32", "fop = tef", "ctx = stm");
	} else
		fd = open(name, m, 0666, "mbc = 32", "ctx = stm");
	if (fd < 0) {
		TIFFErrorExt(0, module, "%s: Cannot open", name);
		return ((TIFF*)0);
	}
	return (TIFFFdOpen(fd, name, mode));
}
