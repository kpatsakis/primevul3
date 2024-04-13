static void add_cx(MCInst *MI)
{
	if (MI->csh->detail) {
		x86_reg cx;

		if (MI->csh->mode & CS_MODE_16)
			cx = X86_REG_CX;
		else if (MI->csh->mode & CS_MODE_32)
			cx = X86_REG_ECX;
		else	// 64-bit
			cx = X86_REG_RCX;

		MI->flat_insn->detail->regs_read[MI->flat_insn->detail->regs_read_count] = cx;
		MI->flat_insn->detail->regs_read_count++;

		MI->flat_insn->detail->regs_write[MI->flat_insn->detail->regs_write_count] = cx;
		MI->flat_insn->detail->regs_write_count++;
	}
}
