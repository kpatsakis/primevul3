TIFFOpen(const char* name, const char* mode)
{
	static const char module[] = "TIFFOpen";
	int m, fd;
	TIFF* tif;

	m = _TIFFgetMode(mode, module);
	if (m == -1)
		return ((TIFF*)0);

/* for cygwin and mingw */
#ifdef O_BINARY
	m |= O_BINARY;
#endif

	fd = open(name, m, 0666);
	if (fd < 0) {
		if (errno > 0 && strerror(errno) != NULL ) {
			TIFFErrorExt(0, module, "%s: %s", name, strerror(errno) );
		} else {
			TIFFErrorExt(0, module, "%s: Cannot open", name);
		}
		return ((TIFF *)0);
	}

	tif = TIFFFdOpen((int)fd, name, mode);
	if(!tif)
		close(fd);
	return tif;
}
