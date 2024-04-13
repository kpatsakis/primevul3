bool X86_insn_reg_intel2(unsigned int id, x86_reg *reg1, enum cs_ac_type *access1, x86_reg *reg2, enum cs_ac_type *access2)
{
	unsigned int i;

	for (i = 0; i < ARR_SIZE(insn_regs_intel2); i++) {
		if (insn_regs_intel2[i].insn == id) {
			*reg1 = insn_regs_intel2[i].reg1;
			*reg2 = insn_regs_intel2[i].reg2;
			if (access1)
				*access1 = insn_regs_intel2[i].access1;
			if (access2)
				*access2 = insn_regs_intel2[i].access2;
			return true;
		}
	}

	return false;
}
