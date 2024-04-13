static int opxadd(RAsm *a, ut8 *data, const Opcode *op) {
	int i = 0;
	if (op->operands_count < 2 ) {
		return -1;
	}
	if (a->bits == 64) {
		data[i++] = 0x48;
	};
	data[i++] = 0x0f;
	if (op->operands[0].type & OT_BYTE &&
	   op->operands[1].type & OT_BYTE) {
		data[i++] = 0xc0;
	} else {
		data[i++] = 0xc1;
	}
	if (op->operands[0].type & OT_REGALL &&
       op->operands[1].type & OT_REGALL) { // TODO memory modes
		data[i] |= 0xc0;
		data[i] |= (op->operands[1].reg << 3);
		data[i++] |= op->operands[0].reg;
	}
	return i;
}
