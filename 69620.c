_tiffSizeProc(thandle_t fd)
{
	struct stat sb;
	if (fstat((int)fd,&sb)<0)
		return(0);
	else
		return((uint64)sb.st_size);
}
