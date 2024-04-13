SYSCALL_DEFINE3(recvmsg, int, fd, struct user_msghdr __user *, msg,
		unsigned int, flags)
{
	return __sys_recvmsg(fd, msg, flags, true);
}
