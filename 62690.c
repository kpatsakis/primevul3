void SMIException(struct pt_regs *regs)
{
	die("System Management Interrupt", regs, SIGABRT);
}
