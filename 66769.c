SYSCALL_DEFINE5(setsockopt, int, fd, int, level, int, optname,
		char __user *, optval, int, optlen)
{
	return __sys_setsockopt(fd, level, optname, optval, optlen);
}
