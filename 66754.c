SYSCALL_DEFINE2(listen, int, fd, int, backlog)
{
	return __sys_listen(fd, backlog);
}
