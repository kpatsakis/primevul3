__visible void __noreturn handle_stack_overflow(const char *message,
						struct pt_regs *regs,
						unsigned long fault_address)
{
	printk(KERN_EMERG "BUG: stack guard page was hit at %p (stack is %p..%p)\n",
		 (void *)fault_address, current->stack,
		 (char *)current->stack + THREAD_SIZE - 1);
	die(message, regs, 0);

	/* Be absolutely certain we don't return. */
	panic(message);
}
