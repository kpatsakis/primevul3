dotraplinkage void do_double_fault(struct pt_regs *regs, long error_code)
{
	static const char str[] = "double fault";
	struct task_struct *tsk = current;
#ifdef CONFIG_VMAP_STACK
	unsigned long cr2;
#endif

#ifdef CONFIG_X86_ESPFIX64
	extern unsigned char native_irq_return_iret[];

	/*
	 * If IRET takes a non-IST fault on the espfix64 stack, then we
	 * end up promoting it to a doublefault.  In that case, take
	 * advantage of the fact that we're not using the normal (TSS.sp0)
	 * stack right now.  We can write a fake #GP(0) frame at TSS.sp0
	 * and then modify our own IRET frame so that, when we return,
	 * we land directly at the #GP(0) vector with the stack already
	 * set up according to its expectations.
	 *
	 * The net result is that our #GP handler will think that we
	 * entered from usermode with the bad user context.
	 *
	 * No need for ist_enter here because we don't use RCU.
	 */
	if (((long)regs->sp >> P4D_SHIFT) == ESPFIX_PGD_ENTRY &&
		regs->cs == __KERNEL_CS &&
		regs->ip == (unsigned long)native_irq_return_iret)
	{
		struct pt_regs *gpregs = (struct pt_regs *)this_cpu_read(cpu_tss_rw.x86_tss.sp0) - 1;

		/*
		 * regs->sp points to the failing IRET frame on the
		 * ESPFIX64 stack.  Copy it to the entry stack.  This fills
		 * in gpregs->ss through gpregs->ip.
		 *
		 */
		memmove(&gpregs->ip, (void *)regs->sp, 5*8);
		gpregs->orig_ax = 0;  /* Missing (lost) #GP error code */

		/*
		 * Adjust our frame so that we return straight to the #GP
		 * vector with the expected RSP value.  This is safe because
		 * we won't enable interupts or schedule before we invoke
		 * general_protection, so nothing will clobber the stack
		 * frame we just set up.
		 */
		regs->ip = (unsigned long)general_protection;
		regs->sp = (unsigned long)&gpregs->orig_ax;

		return;
	}
#endif

	ist_enter(regs);
	notify_die(DIE_TRAP, str, regs, error_code, X86_TRAP_DF, SIGSEGV);

	tsk->thread.error_code = error_code;
	tsk->thread.trap_nr = X86_TRAP_DF;

#ifdef CONFIG_VMAP_STACK
	/*
	 * If we overflow the stack into a guard page, the CPU will fail
	 * to deliver #PF and will send #DF instead.  Similarly, if we
	 * take any non-IST exception while too close to the bottom of
	 * the stack, the processor will get a page fault while
	 * delivering the exception and will generate a double fault.
	 *
	 * According to the SDM (footnote in 6.15 under "Interrupt 14 -
	 * Page-Fault Exception (#PF):
	 *
	 *   Processors update CR2 whenever a page fault is detected. If a
	 *   second page fault occurs while an earlier page fault is being
	 *   delivered, the faulting linear address of the second fault will
	 *   overwrite the contents of CR2 (replacing the previous
	 *   address). These updates to CR2 occur even if the page fault
	 *   results in a double fault or occurs during the delivery of a
	 *   double fault.
	 *
	 * The logic below has a small possibility of incorrectly diagnosing
	 * some errors as stack overflows.  For example, if the IDT or GDT
	 * gets corrupted such that #GP delivery fails due to a bad descriptor
	 * causing #GP and we hit this condition while CR2 coincidentally
	 * points to the stack guard page, we'll think we overflowed the
	 * stack.  Given that we're going to panic one way or another
	 * if this happens, this isn't necessarily worth fixing.
	 *
	 * If necessary, we could improve the test by only diagnosing
	 * a stack overflow if the saved RSP points within 47 bytes of
	 * the bottom of the stack: if RSP == tsk_stack + 48 and we
	 * take an exception, the stack is already aligned and there
	 * will be enough room SS, RSP, RFLAGS, CS, RIP, and a
	 * possible error code, so a stack overflow would *not* double
	 * fault.  With any less space left, exception delivery could
	 * fail, and, as a practical matter, we've overflowed the
	 * stack even if the actual trigger for the double fault was
	 * something else.
	 */
	cr2 = read_cr2();
	if ((unsigned long)task_stack_page(tsk) - 1 - cr2 < PAGE_SIZE)
		handle_stack_overflow("kernel stack overflow (double-fault)", regs, cr2);
#endif

#ifdef CONFIG_DOUBLEFAULT
	df_debug(regs, error_code);
#endif
	/*
	 * This is always a kernel trap and never fixable (and thus must
	 * never return).
	 */
	for (;;)
		die(str, regs, error_code);
}
