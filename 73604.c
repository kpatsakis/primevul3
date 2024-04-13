static bool valid_rep(cs_struct *h, unsigned int opcode)
{
	unsigned int id;
	int i = insn_find(insns, ARR_SIZE(insns), opcode, &h->insn_cache);
	if (i != 0) {
		id = insns[i].mapid;
		switch(id) {
			default:
				return false;

			case X86_INS_MOVSB:
			case X86_INS_MOVSW:
			case X86_INS_MOVSQ:

			case X86_INS_LODSB:
			case X86_INS_LODSW:
			case X86_INS_LODSQ:

			case X86_INS_STOSB:
			case X86_INS_STOSW:
			case X86_INS_STOSQ:

			case X86_INS_INSB:
			case X86_INS_INSW:
			case X86_INS_INSD:

			case X86_INS_OUTSB:
			case X86_INS_OUTSW:
			case X86_INS_OUTSD:
				return true;

			case X86_INS_MOVSD:
				if (opcode == X86_MOVSL) // REP MOVSD
					return true;
				return false;

			case X86_INS_LODSD:
				if (opcode == X86_LODSL) // REP LODSD
					return true;
				return false;

			case X86_INS_STOSD:
				if (opcode == X86_STOSL) // REP STOSD
					return true;
				return false;
		}
	}

	return false;
}
