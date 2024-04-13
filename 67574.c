struct bad_iret_stack *fixup_bad_iret(struct bad_iret_stack *s)
{
	/*
	 * This is called from entry_64.S early in handling a fault
	 * caused by a bad iret to user mode.  To handle the fault
	 * correctly, we want to move our stack frame to where it would
	 * be had we entered directly on the entry stack (rather than
	 * just below the IRET frame) and we want to pretend that the
	 * exception came from the IRET target.
	 */
	struct bad_iret_stack *new_stack =
		(struct bad_iret_stack *)this_cpu_read(cpu_tss_rw.x86_tss.sp0) - 1;

	/* Copy the IRET target to the new stack. */
	memmove(&new_stack->regs.ip, (void *)s->regs.sp, 5*8);

	/* Copy the remainder of the stack from the current stack. */
	memmove(new_stack, s, offsetof(struct bad_iret_stack, regs.ip));

	BUG_ON(!user_mode(&new_stack->regs));
	return new_stack;
}
