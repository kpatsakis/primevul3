void op_addImm(MCInst *MI, int v)
{
	if (MI->csh->detail) {
		MI->flat_insn->detail->x86.operands[MI->flat_insn->detail->x86.op_count].type = X86_OP_IMM;
		MI->flat_insn->detail->x86.operands[MI->flat_insn->detail->x86.op_count].imm = v;
		if (MI->csh->syntax != CS_OPT_SYNTAX_ATT) {
			if (MI->flat_insn->detail->x86.op_count > 0)
				MI->flat_insn->detail->x86.operands[MI->flat_insn->detail->x86.op_count].size = MI->flat_insn->detail->x86.operands[0].size;
			else
				MI->flat_insn->detail->x86.operands[MI->flat_insn->detail->x86.op_count].size = MI->imm_size;
		} else
			MI->has_imm = true;
		MI->flat_insn->detail->x86.op_count++;
	}

	if (MI->op1_size == 0)
		MI->op1_size = MI->imm_size;
}
