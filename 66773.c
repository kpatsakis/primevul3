SYSCALL_DEFINE6(recvfrom, int, fd, void __user *, ubuf, size_t, size,
		unsigned int, flags, struct sockaddr __user *, addr,
		int __user *, addr_len)
{
	return __sys_recvfrom(fd, ubuf, size, flags, addr, addr_len);
}
