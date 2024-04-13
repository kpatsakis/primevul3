void StackOverflow(struct pt_regs *regs)
{
	printk(KERN_CRIT "Kernel stack overflow in process %p, r1=%lx\n",
	       current, regs->gpr[1]);
	debugger(regs);
	show_regs(regs);
	panic("kernel stack overflow");
}
