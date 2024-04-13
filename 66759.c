SYSCALL_DEFINE3(accept, int, fd, struct sockaddr __user *, upeer_sockaddr,
		int __user *, upeer_addrlen)
{
	return __sys_accept4(fd, upeer_sockaddr, upeer_addrlen, 0);
}
