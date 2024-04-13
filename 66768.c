SYSCALL_DEFINE4(sendmmsg, int, fd, struct mmsghdr __user *, mmsg,
		unsigned int, vlen, unsigned int, flags)
{
	return __sys_sendmmsg(fd, mmsg, vlen, flags, true);
}
