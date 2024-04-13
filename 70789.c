int ptrace_writedata(struct task_struct *tsk, char __user *src, unsigned long dst, int len)
{
	int copied = 0;

	while (len > 0) {
		char buf[128];
		int this_len, retval;

		this_len = (len > sizeof(buf)) ? sizeof(buf) : len;
		if (copy_from_user(buf, src, this_len))
			return -EFAULT;
		retval = ptrace_access_vm(tsk, dst, buf, this_len,
				FOLL_FORCE | FOLL_WRITE);
		if (!retval) {
			if (copied)
				break;
			return -EIO;
		}
		copied += retval;
		src += retval;
		dst += retval;
		len -= retval;
	}
	return copied;
}
