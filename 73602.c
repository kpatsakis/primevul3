void op_addSseCC(MCInst *MI, int v)
{
	if (MI->csh->detail) {
		MI->flat_insn->detail->x86.sse_cc = v;
	}
}
