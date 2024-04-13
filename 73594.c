const char *X86_reg_name(csh handle, unsigned int reg)
{
#ifndef CAPSTONE_DIET
	cs_struct *ud = (cs_struct *)handle;

	if (reg >= X86_REG_ENDING)
		return NULL;

	if (reg == X86_REG_EFLAGS) {
		if (ud->mode & CS_MODE_32)
			return "eflags";
		if (ud->mode & CS_MODE_64)
			return "rflags";
	}

	return reg_name_maps[reg].name;
#else
	return NULL;
#endif
}
