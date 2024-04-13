void CacheLockingException(struct pt_regs *regs, unsigned long address,
			   unsigned long error_code)
{
	/* We treat cache locking instructions from the user
	 * as priv ops, in the future we could try to do
	 * something smarter
	 */
	if (error_code & (ESR_DLK|ESR_ILK))
		_exception(SIGILL, regs, ILL_PRVOPC, regs->nip);
	return;
}
