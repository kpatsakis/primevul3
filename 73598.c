void op_addAvxRoundingMode(MCInst *MI, int v)
{
	if (MI->csh->detail) {
		MI->flat_insn->detail->x86.avx_rm = v;
	}
}
