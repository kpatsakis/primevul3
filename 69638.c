_tiffSizeProc(thandle_t fd)
{
	struct stat sb;
	return (toff_t) (fstat((int) fd, &sb) < 0 ? 0 : sb.st_size);
}
