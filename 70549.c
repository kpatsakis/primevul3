static void wait_message(time_t tmo)
{
	int num = 1, timeout;
	size_t ifnum = 0;
	struct pollfd pfd[MAX_NUM_IFACES];
	struct ifsock *ifs;

	LIST_FOREACH(ifs, &il, link) {
		if (ifs->out != -1)
			continue;

		pfd[ifnum].fd = ifs->in;
		pfd[ifnum].events = POLLIN | POLLHUP;
		ifnum++;
	}

	while (1) {
		size_t i;

		timeout = tmo - time(NULL);
		if (timeout < 0)
			break;

		num = poll(pfd, ifnum, timeout * 1000);
		if (num < 0) {
			if (EINTR == errno)
				break;

			err(1, "Unrecoverable error");
		}

		if (num == 0)
			break;

		for (i = 0; num > 0 && i < ifnum; i++) {
			if (pfd[i].revents & POLLIN) {
				handle_message(pfd[i].fd);
				num--;
			}
		}
	}
}
