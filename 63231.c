static int opxor(RAsm *a, ut8 * data, const Opcode *op) {
	if (op->operands_count < 2) {
		return -1;
	}
	if (op->operands[0].type == 0x80 && op->operands[0].reg == X86R_UNDEFINED) {
		return -1;
	}
	if (op->operands[1].type == 0x80 && op->operands[0].reg == X86R_UNDEFINED) {
		return -1;
	}
	if (op->operands[1].type & OT_CONSTANT) {
		if (op->operands[0].type & OT_GPREG &&
		    op->operands[0].type & OT_WORD) {
			return process_16bit_group_1 (a, data, op, 0x30);
		}
		if (!is_al_reg (&op->operands[0])) {
			return process_group_1 (a, data, op);
		}
	}
	return process_1byte_op (a, data, op, 0x30);
}
