_tiffWriteProc(thandle_t fd, void* buf, tmsize_t size)
{
	size_t size_io = (size_t) size;
	if ((tmsize_t) size_io != size)
	{
		errno=EINVAL;
		return (tmsize_t) -1;
	}
	return ((tmsize_t) write((int) fd, buf, size_io));
}
