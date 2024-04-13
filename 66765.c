SYSCALL_DEFINE4(accept4, int, fd, struct sockaddr __user *, upeer_sockaddr,
		int __user *, upeer_addrlen, int, flags)
{
	return __sys_accept4(fd, upeer_sockaddr, upeer_addrlen, flags);
}
