SYSCALL_DEFINE2(shutdown, int, fd, int, how)
{
	return __sys_shutdown(fd, how);
}
