void system_reset_exception(struct pt_regs *regs)
{
	/* See if any machine dependent calls */
	if (ppc_md.system_reset_exception) {
		if (ppc_md.system_reset_exception(regs))
			return;
	}

#ifdef CONFIG_KEXEC
	cpu_set(smp_processor_id(), cpus_in_sr);
#endif

	die("System Reset", regs, SIGABRT);

	/*
	 * Some CPUs when released from the debugger will execute this path.
	 * These CPUs entered the debugger via a soft-reset. If the CPU was
	 * hung before entering the debugger it will return to the hung
	 * state when exiting this function.  This causes a problem in
	 * kdump since the hung CPU(s) will not respond to the IPI sent
	 * from kdump. To prevent the problem we call crash_kexec_secondary()
	 * here. If a kdump had not been initiated or we exit the debugger
	 * with the "exit and recover" command (x) crash_kexec_secondary()
	 * will return after 5ms and the CPU returns to its previous state.
	 */
	crash_kexec_secondary(regs);

	/* Must die if the interrupt is not recoverable */
	if (!(regs->msr & MSR_RI))
		panic("Unrecoverable System Reset");

	/* What should we do here? We could issue a shutdown or hard reset. */
}
