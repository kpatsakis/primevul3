bool X86_lockrep(MCInst *MI, SStream *O)
{
	unsigned int opcode;
	bool res = false;

	switch(MI->x86_prefix[0]) {
		default:
			break;
		case 0xf0:
#ifndef CAPSTONE_DIET
			SStream_concat(O, "lock|");
#endif
			break;
		case 0xf2:	// repne
			opcode = MCInst_getOpcode(MI);
#ifndef CAPSTONE_DIET	// only care about memonic in standard (non-diet) mode
			if (valid_repne(MI->csh, opcode)) {
				SStream_concat(O, "repne|");
				add_cx(MI);
			} else {
				MI->x86_prefix[0] = 0;

#ifndef CAPSTONE_X86_REDUCE
				if (opcode == X86_MULPDrr) {
					MCInst_setOpcode(MI, X86_MULSDrr);
					SStream_concat(O, "mulsd\t");
					res = true;
				}
#endif
			}
#else	// diet mode -> only patch opcode in special cases
			if (!valid_repne(MI->csh, opcode)) {
				MI->x86_prefix[0] = 0;
			}
#ifndef CAPSTONE_X86_REDUCE
			if (opcode == X86_MULPDrr) {
				MCInst_setOpcode(MI, X86_MULSDrr);
			}
#endif
#endif
			break;

		case 0xf3:
			opcode = MCInst_getOpcode(MI);
#ifndef CAPSTONE_DIET	// only care about memonic in standard (non-diet) mode
			if (valid_rep(MI->csh, opcode)) {
				SStream_concat(O, "rep|");
				add_cx(MI);
			} else if (valid_repe(MI->csh, opcode)) {
				SStream_concat(O, "repe|");
				add_cx(MI);
			} else {
				MI->x86_prefix[0] = 0;

#ifndef CAPSTONE_X86_REDUCE
				if (opcode == X86_MULPDrr) {
					MCInst_setOpcode(MI, X86_MULSSrr);
					SStream_concat(O, "mulss\t");
					res = true;
				}
#endif
			}
#else	// diet mode -> only patch opcode in special cases
			if (!valid_rep(MI->csh, opcode) && !valid_repe(MI->csh, opcode)) {
				MI->x86_prefix[0] = 0;
			}
#ifndef CAPSTONE_X86_REDUCE
			if (opcode == X86_MULPDrr) {
				MCInst_setOpcode(MI, X86_MULSSrr);
			}
#endif
#endif
			break;
	}

	if (MI->csh->detail)
		memcpy(MI->flat_insn->detail->x86.prefix, MI->x86_prefix, ARR_SIZE(MI->x86_prefix));

	return res;
}
