SYSCALL_DEFINE4(recv, int, fd, void __user *, ubuf, size_t, size,
		unsigned int, flags)
{
	return __sys_recvfrom(fd, ubuf, size, flags, NULL, NULL);
}
