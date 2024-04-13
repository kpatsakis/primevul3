INST_HANDLER (eijmp) {	// EIJMP
	ut64 z, eind;
	r_anal_esil_reg_read (anal->esil, "z",    &z,    NULL);
	r_anal_esil_reg_read (anal->esil, "eind", &eind, NULL);
	op->jump = ((eind << 16) + z) << 1;
	ESIL_A ("1,z,16,eind,<<,+,<<,pc,=,");
	op->cycles = 2;
}
