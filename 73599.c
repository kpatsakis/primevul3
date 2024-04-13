void op_addAvxSae(MCInst *MI)
{
	if (MI->csh->detail) {
		MI->flat_insn->detail->x86.avx_sae = true;
	}
}
