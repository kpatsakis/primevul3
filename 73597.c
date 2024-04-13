void op_addAvxBroadcast(MCInst *MI, x86_avx_bcast v)
{
	if (MI->csh->detail) {
		MI->flat_insn->detail->x86.operands[MI->flat_insn->detail->x86.op_count - 1].avx_bcast = v;
	}
}
