INST_HANDLER (ijmp) {	// IJMP k
	ut64 z;
	r_anal_esil_reg_read (anal->esil, "z", &z, NULL);
	op->jump = z << 1;
	op->cycles = 2;
	ESIL_A ("1,z,<<,pc,=,");		// jump!
}
