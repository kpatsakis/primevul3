static int opint(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	if (op->operands[0].type & OT_CONSTANT) {
		st32 immediate = op->operands[0].immediate * op->operands[0].sign;
		if (immediate <= 255 && immediate >= -128) {
			data[l++] = 0xcd;
			data[l++] = immediate;
		}
	}
	return l;
}
