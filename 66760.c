SYSCALL_DEFINE3(connect, int, fd, struct sockaddr __user *, uservaddr,
		int, addrlen)
{
	return __sys_connect(fd, uservaddr, addrlen);
}
