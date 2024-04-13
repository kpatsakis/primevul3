_tiffUnmapProc(thandle_t fd, void* base, toff_t size)
{
	(void) fd;
	(void) munmap(base, (off_t) size);
}
