static int emulate_popcntb_inst(struct pt_regs *regs, u32 instword)
{
	u32 ra,rs;
	unsigned long tmp;

	ra = (instword >> 16) & 0x1f;
	rs = (instword >> 21) & 0x1f;

	tmp = regs->gpr[rs];
	tmp = tmp - ((tmp >> 1) & 0x5555555555555555ULL);
	tmp = (tmp & 0x3333333333333333ULL) + ((tmp >> 2) & 0x3333333333333333ULL);
	tmp = (tmp + (tmp >> 4)) & 0x0f0f0f0f0f0f0f0fULL;
	regs->gpr[ra] = tmp;

	return 0;
}
