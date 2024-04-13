dotraplinkage void do_debug(struct pt_regs *regs, long error_code)
{
	struct task_struct *tsk = current;
	int user_icebp = 0;
	unsigned long dr6;
	int si_code;

	ist_enter(regs);

	get_debugreg(dr6, 6);
	/*
	 * The Intel SDM says:
	 *
	 *   Certain debug exceptions may clear bits 0-3. The remaining
	 *   contents of the DR6 register are never cleared by the
	 *   processor. To avoid confusion in identifying debug
	 *   exceptions, debug handlers should clear the register before
	 *   returning to the interrupted task.
	 *
	 * Keep it simple: clear DR6 immediately.
	 */
	set_debugreg(0, 6);

	/* Filter out all the reserved bits which are preset to 1 */
	dr6 &= ~DR6_RESERVED;

	/*
	 * The SDM says "The processor clears the BTF flag when it
	 * generates a debug exception."  Clear TIF_BLOCKSTEP to keep
	 * TIF_BLOCKSTEP in sync with the hardware BTF flag.
	 */
	clear_tsk_thread_flag(tsk, TIF_BLOCKSTEP);

	if (unlikely(!user_mode(regs) && (dr6 & DR_STEP) &&
		     is_sysenter_singlestep(regs))) {
		dr6 &= ~DR_STEP;
		if (!dr6)
			goto exit;
		/*
		 * else we might have gotten a single-step trap and hit a
		 * watchpoint at the same time, in which case we should fall
		 * through and handle the watchpoint.
		 */
	}

	/*
	 * If dr6 has no reason to give us about the origin of this trap,
	 * then it's very likely the result of an icebp/int01 trap.
	 * User wants a sigtrap for that.
	 */
	if (!dr6 && user_mode(regs))
		user_icebp = 1;

	/* Store the virtualized DR6 value */
	tsk->thread.debugreg6 = dr6;

#ifdef CONFIG_KPROBES
	if (kprobe_debug_handler(regs))
		goto exit;
#endif

	if (notify_die(DIE_DEBUG, "debug", regs, (long)&dr6, error_code,
							SIGTRAP) == NOTIFY_STOP)
		goto exit;

	/*
	 * Let others (NMI) know that the debug stack is in use
	 * as we may switch to the interrupt stack.
	 */
	debug_stack_usage_inc();

	/* It's safe to allow irq's after DR6 has been saved */
	cond_local_irq_enable(regs);

	if (v8086_mode(regs)) {
		handle_vm86_trap((struct kernel_vm86_regs *) regs, error_code,
					X86_TRAP_DB);
		cond_local_irq_disable(regs);
		debug_stack_usage_dec();
		goto exit;
	}

	if (WARN_ON_ONCE((dr6 & DR_STEP) && !user_mode(regs))) {
		/*
		 * Historical junk that used to handle SYSENTER single-stepping.
		 * This should be unreachable now.  If we survive for a while
		 * without anyone hitting this warning, we'll turn this into
		 * an oops.
		 */
		tsk->thread.debugreg6 &= ~DR_STEP;
		set_tsk_thread_flag(tsk, TIF_SINGLESTEP);
		regs->flags &= ~X86_EFLAGS_TF;
	}
	si_code = get_si_code(tsk->thread.debugreg6);
	if (tsk->thread.debugreg6 & (DR_STEP | DR_TRAP_BITS) || user_icebp)
		send_sigtrap(tsk, regs, error_code, si_code);
	cond_local_irq_disable(regs);
	debug_stack_usage_dec();

exit:
	ist_exit(regs);
}
