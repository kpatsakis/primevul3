SYSCALL_DEFINE5(recvmmsg, int, fd, struct mmsghdr __user *, mmsg,
		unsigned int, vlen, unsigned int, flags,
		struct timespec __user *, timeout)
{
	return do_sys_recvmmsg(fd, mmsg, vlen, flags, timeout);
}
