INST_HANDLER (rcall) {	// RCALL k
	op->jump = (op->addr
		+ (((((buf[1] & 0xf) << 8) | buf[0]) << 1)
			| (((buf[1] & 0x8) ? ~((int) 0x1fff) : 0)))
		+ 2) & CPU_PC_MASK (cpu);
	op->fail = op->addr + op->size;
	ESIL_A ("pc,");				// esil already points to next
	__generic_push (op, CPU_PC_SIZE (cpu));	// push @ret addr
	ESIL_A ("%"PFMT64d",pc,=,", op->jump);	// jump!
	if (!strncasecmp (cpu->model, "ATtiny", 6)) {
		op->cycles = 4;	// ATtiny is always slow
	} else {
		op->cycles = cpu->pc <= 16 ? 3 : 4;
		if (!STR_BEGINS (cpu->model, "ATxmega")) {
			op->cycles--;	// ATxmega optimizes one cycle
		}
	}
}
