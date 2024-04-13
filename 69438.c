writen(int fd, const void *buf, ssize_t len) {
	int	nleft;
	int	nwritten;
	char *	buffer = (char *)buf;

	nleft = len;
	while (nleft > 0) {
		nwritten = write(fd, buffer, len);

		if (nwritten < 0) {
			if (errno == EINTR || errno == EAGAIN)
				continue;
			else {
				LOG_ERRNO(LOG_ERR, ("write failed"));
				return -1;
			}
		} else {
			nleft -= nwritten;
			buffer += nwritten;
		}
	}

	LOG(LOG_DEBUG, ("wrote %d bytes", len));
	return len;
}
