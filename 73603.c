static int regs_cmp(const void *a, const void *b)
{
	uint16_t l = ((struct insn_reg *)a)->insn;
	uint16_t r = ((struct insn_reg *)b)->insn;
 	return (l - r);
 }
