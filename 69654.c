_tiffMapProc(thandle_t fd, void** pbase, toff_t* psize)
{
	uint64 size;
	tmsize_t sizem;
	HANDLE hMapFile;

	size = _tiffSizeProc(fd);
	sizem = (tmsize_t)size;
	if ((uint64)sizem!=size)
		return (0);

	/* By passing in 0 for the maximum file size, it specifies that we
	   create a file mapping object for the full file size. */
	hMapFile = CreateFileMapping(fd, NULL, PAGE_READONLY, 0, 0, NULL);
	if (hMapFile == NULL)
		return (0);
	*pbase = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
	CloseHandle(hMapFile);
	if (*pbase == NULL)
		return (0);
	*psize = size;
	return(1);
}
