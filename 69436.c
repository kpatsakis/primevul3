timed_read(int fd, void *buf, size_t bytes, int timeout)
{
	struct pollfd		fds[1];
	int			ret;

	if (timeout > 0) {
		fds[0].fd = fd;
		fds[0].events = POLLIN;

		ret = poll(fds, 1, timeout);

		if (ret == -1)
			return -1;

		if (ret != 1) {
			errno = ETIMEDOUT;
			return -1;
		}
	}

	return read(fd, buf, bytes);
}
