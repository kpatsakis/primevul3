void _exception(int signr, struct pt_regs *regs, int code, unsigned long addr)
{
	siginfo_t info;

	if (!user_mode(regs)) {
		if (die("Exception in kernel mode", regs, signr))
			return;
	}

	memset(&info, 0, sizeof(info));
	info.si_signo = signr;
	info.si_code = code;
	info.si_addr = (void __user *) addr;
	force_sig_info(signr, &info, current);

	/*
	 * Init gets no signals that it doesn't have a handler for.
	 * That's all very well, but if it has caused a synchronous
	 * exception and we ignore the resulting signal, it will just
	 * generate the same exception over and over again and we get
	 * nowhere.  Better to kill it and let the kernel panic.
	 */
	if (current->pid == 1) {
		__sighandler_t handler;

		spin_lock_irq(&current->sighand->siglock);
		handler = current->sighand->action[signr-1].sa.sa_handler;
		spin_unlock_irq(&current->sighand->siglock);
		if (handler == SIG_DFL) {
			/* init has generated a synchronous exception
			   and it doesn't have a handler for the signal */
			printk(KERN_CRIT "init has generated signal %d "
			       "but has no handler for it\n", signr);
			do_exit(signr);
		}
	}
}
