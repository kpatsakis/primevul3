SYSCALL_DEFINE4(socketpair, int, family, int, type, int, protocol,
		int __user *, usockvec)
{
	return __sys_socketpair(family, type, protocol, usockvec);
}
