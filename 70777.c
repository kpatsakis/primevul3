int generic_ptrace_peekdata(struct task_struct *tsk, unsigned long addr,
			    unsigned long data)
{
	unsigned long tmp;
	int copied;

	copied = ptrace_access_vm(tsk, addr, &tmp, sizeof(tmp), FOLL_FORCE);
	if (copied != sizeof(tmp))
		return -EIO;
	return put_user(tmp, (unsigned long __user *)data);
}
