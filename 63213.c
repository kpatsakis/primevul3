static int opretf(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	st32 immediate = 0;
	if (op->operands[0].type & OT_CONSTANT) {
		immediate = op->operands[0].immediate * op->operands[0].sign;
		data[l++] = 0xca;
		data[l++] = immediate;
		data[l++] = immediate >> 8;
	} else if (op->operands[0].type == OT_UNKNOWN) {
		data[l++] = 0xcb;
	}
	return l;
}
