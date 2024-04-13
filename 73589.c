const char *X86_insn_name(csh handle, unsigned int id)
{
#ifndef CAPSTONE_DIET
	if (id >= X86_INS_ENDING)
		return NULL;

	return insn_name_maps[id].name;
#else
	return NULL;
#endif
}
