void op_addReg(MCInst *MI, int reg)
{
	if (MI->csh->detail) {
		MI->flat_insn->detail->x86.operands[MI->flat_insn->detail->x86.op_count].type = X86_OP_REG;
		MI->flat_insn->detail->x86.operands[MI->flat_insn->detail->x86.op_count].reg = reg;
		MI->flat_insn->detail->x86.operands[MI->flat_insn->detail->x86.op_count].size = MI->csh->regsize_map[reg];
		MI->flat_insn->detail->x86.op_count++;
	}

	if (MI->op1_size == 0)
		MI->op1_size = MI->csh->regsize_map[reg];
}
