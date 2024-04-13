_tiffCloseProc(thandle_t fd)
{
	return (CloseHandle(fd) ? 0 : -1);
}
