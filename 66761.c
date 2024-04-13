SYSCALL_DEFINE3(getsockname, int, fd, struct sockaddr __user *, usockaddr,
		int __user *, usockaddr_len)
{
	return __sys_getsockname(fd, usockaddr, usockaddr_len);
}
