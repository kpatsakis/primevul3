SYSCALL_DEFINE3(getpeername, int, fd, struct sockaddr __user *, usockaddr,
		int __user *, usockaddr_len)
{
	return __sys_getpeername(fd, usockaddr, usockaddr_len);
}
