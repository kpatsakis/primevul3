COMPAT_SYSCALL_DEFINE1(sigpending, compat_old_sigset_t __user *, set32)
{
	sigset_t set;
	int err = do_sigpending(&set, sizeof(old_sigset_t)); 
	if (err == 0)
		if (copy_to_user(set32, &set, sizeof(old_sigset_t)))
			err = -EFAULT;
	return err;
}
