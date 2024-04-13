SYSCALL_DEFINE5(getsockopt, int, fd, int, level, int, optname,
		char __user *, optval, int __user *, optlen)
{
	return __sys_getsockopt(fd, level, optname, optval, optlen);
}
