x86_reg X86_insn_reg_att(unsigned int id, enum cs_ac_type *access)
{
	unsigned int i;

	for (i = 0; i < ARR_SIZE(insn_regs_att); i++) {
		if (insn_regs_att[i].insn == id) {
            if (access)
                *access = insn_regs_intel[i].access;
			return insn_regs_att[i].reg;
		}
	}

	return 0;
}
