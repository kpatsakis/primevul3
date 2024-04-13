_tiffWriteProc(thandle_t fd, tdata_t buf, tsize_t size)
{
	return (write((int) fd, buf, size));
}
