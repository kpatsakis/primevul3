evutil_open_closeonexec_(const char *pathname, int flags, unsigned mode)
{
	int fd;

#ifdef O_CLOEXEC
	fd = open(pathname, flags|O_CLOEXEC, (mode_t)mode);
	if (fd >= 0 || errno == EINVAL)
		return fd;
	/* If we got an EINVAL, fall through and try without O_CLOEXEC */
#endif
	fd = open(pathname, flags, (mode_t)mode);
	if (fd < 0)
		return -1;

#if defined(FD_CLOEXEC)
	if (fcntl(fd, F_SETFD, FD_CLOEXEC) < 0) {
		close(fd);
		return -1;
	}
#endif

	return fd;
}
