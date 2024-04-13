SYSCALL_DEFINE2(socketcall, int, call, unsigned long __user *, args)
{
	unsigned long a[AUDITSC_ARGS];
	unsigned long a0, a1;
	int err;
	unsigned int len;

	if (call < 1 || call > SYS_SENDMMSG)
		return -EINVAL;

	len = nargs[call];
	if (len > sizeof(a))
		return -EINVAL;

	/* copy_from_user should be SMP safe. */
	if (copy_from_user(a, args, len))
		return -EFAULT;

	err = audit_socketcall(nargs[call] / sizeof(unsigned long), a);
	if (err)
		return err;

	a0 = a[0];
	a1 = a[1];

	switch (call) {
	case SYS_SOCKET:
		err = __sys_socket(a0, a1, a[2]);
		break;
	case SYS_BIND:
		err = __sys_bind(a0, (struct sockaddr __user *)a1, a[2]);
		break;
	case SYS_CONNECT:
		err = __sys_connect(a0, (struct sockaddr __user *)a1, a[2]);
		break;
	case SYS_LISTEN:
		err = __sys_listen(a0, a1);
		break;
	case SYS_ACCEPT:
		err = __sys_accept4(a0, (struct sockaddr __user *)a1,
				    (int __user *)a[2], 0);
		break;
	case SYS_GETSOCKNAME:
		err =
		    __sys_getsockname(a0, (struct sockaddr __user *)a1,
				      (int __user *)a[2]);
		break;
	case SYS_GETPEERNAME:
		err =
		    __sys_getpeername(a0, (struct sockaddr __user *)a1,
				      (int __user *)a[2]);
		break;
	case SYS_SOCKETPAIR:
		err = __sys_socketpair(a0, a1, a[2], (int __user *)a[3]);
		break;
	case SYS_SEND:
		err = __sys_sendto(a0, (void __user *)a1, a[2], a[3],
				   NULL, 0);
		break;
	case SYS_SENDTO:
		err = __sys_sendto(a0, (void __user *)a1, a[2], a[3],
				   (struct sockaddr __user *)a[4], a[5]);
		break;
	case SYS_RECV:
		err = __sys_recvfrom(a0, (void __user *)a1, a[2], a[3],
				     NULL, NULL);
		break;
	case SYS_RECVFROM:
		err = __sys_recvfrom(a0, (void __user *)a1, a[2], a[3],
				     (struct sockaddr __user *)a[4],
				     (int __user *)a[5]);
		break;
	case SYS_SHUTDOWN:
		err = __sys_shutdown(a0, a1);
		break;
	case SYS_SETSOCKOPT:
		err = __sys_setsockopt(a0, a1, a[2], (char __user *)a[3],
				       a[4]);
		break;
	case SYS_GETSOCKOPT:
		err =
		    __sys_getsockopt(a0, a1, a[2], (char __user *)a[3],
				     (int __user *)a[4]);
		break;
	case SYS_SENDMSG:
		err = __sys_sendmsg(a0, (struct user_msghdr __user *)a1,
				    a[2], true);
		break;
	case SYS_SENDMMSG:
		err = __sys_sendmmsg(a0, (struct mmsghdr __user *)a1, a[2],
				     a[3], true);
		break;
	case SYS_RECVMSG:
		err = __sys_recvmsg(a0, (struct user_msghdr __user *)a1,
				    a[2], true);
		break;
	case SYS_RECVMMSG:
		err = do_sys_recvmmsg(a0, (struct mmsghdr __user *)a1, a[2],
				      a[3], (struct timespec __user *)a[4]);
		break;
	case SYS_ACCEPT4:
		err = __sys_accept4(a0, (struct sockaddr __user *)a1,
				    (int __user *)a[2], a[3]);
		break;
	default:
		err = -EINVAL;
		break;
	}
	return err;
}
