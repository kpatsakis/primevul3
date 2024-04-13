void RunModeException(struct pt_regs *regs)
{
	_exception(SIGTRAP, regs, 0, 0);
}
