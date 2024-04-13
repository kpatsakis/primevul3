INST_HANDLER (ret) {	// RET
	op->eob = true;
	__generic_pop (op, CPU_PC_SIZE (cpu));
	ESIL_A ("pc,=,");	// jump!
	if (CPU_PC_SIZE (cpu) > 2) {	// if we have a bus bigger than 16 bit
		op->cycles++;	// (i.e. a 22-bit bus), add one extra cycle
	}
}
