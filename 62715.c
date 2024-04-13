void trace_syscall(struct pt_regs *regs)
{
	printk("Task: %p(%d), PC: %08lX/%08lX, Syscall: %3ld, Result: %s%ld    %s\n",
	       current, current->pid, regs->nip, regs->link, regs->gpr[0],
	       regs->ccr&0x10000000?"Error=":"", regs->gpr[3], print_tainted());
}
