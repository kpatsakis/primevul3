static bool valid_repe(cs_struct *h, unsigned int opcode)
{
	unsigned int id;
	int i = insn_find(insns, ARR_SIZE(insns), opcode, &h->insn_cache);
	if (i != 0) {
		id = insns[i].mapid;
		switch(id) {
			default:
				return false;

			case X86_INS_CMPSB:
			case X86_INS_CMPSW:
			case X86_INS_CMPSQ:

			case X86_INS_SCASB:
			case X86_INS_SCASW:
			case X86_INS_SCASQ:
				return true;

			case X86_INS_CMPSD:
				if (opcode == X86_CMPSL) // REP CMPSD
					return true;
				return false;

			case X86_INS_SCASD:
				if (opcode == X86_SCASL) // REP SCASD
					return true;
				return false;
		}
	}

	return false;
}
