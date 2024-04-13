static int randomize(pam_handle_t *pamh, unsigned char *r, unsigned int r_len)
{
	int ok = 0;
	int fd = open("/dev/urandom", O_RDONLY);
	if (0 <= fd && read(fd, r, r_len) == (ssize_t)r_len) {
		ok = 1;
	} else {
		pam_syslog(pamh, LOG_CRIT, "Error reading from /dev/urandom: %s",
				strerror(errno));
	}
	if (0 <= fd) {
		close(fd);
	}
	return ok;
}
