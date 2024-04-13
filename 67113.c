INST_HANDLER (icall) {	// ICALL k
	ESIL_A ("pc,");				// esil is already pointing to
	__generic_push (op, CPU_PC_SIZE (cpu));	// push @ret in stack
	INST_CALL (ijmp);
	if (!STR_BEGINS (cpu->model, "ATxmega")) {
		op->cycles--;
	}
}
