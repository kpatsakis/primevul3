_tiffUnmapProc(thandle_t fd, void* base, toff_t size)
{
	(void) fd;
	(void) size;
	UnmapViewOfFile(base);
}
