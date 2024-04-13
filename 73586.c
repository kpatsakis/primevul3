uint8_t *X86_get_op_access(cs_struct *h, unsigned int id, uint64_t *eflags)
{
	int i = insn_find(insns, ARR_SIZE(insns), id, &h->insn_cache);
	if (i != 0) {
		*eflags = insn_ops[i].eflags;
		return insn_ops[i].access;
	}

	return NULL;
}
