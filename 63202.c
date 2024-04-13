static int oples(RAsm *a, ut8* data, const Opcode *op) {
	int l = 0;
	int offset = 0;
	int mod = 0;

	if (op->operands[1].type & OT_MEMORY) {
		data[l++] = 0xc4;
		if (op->operands[1].type & OT_GPREG) {
			offset = op->operands[1].offset * op->operands[1].offset_sign;
			if (offset) {
				mod = 1;
				if (offset > 128 || offset < -128) {
					mod = 2;
				}
			}
			data[l++] = mod << 6 | op->operands[0].reg << 3 | op->operands[1].regs[0];
			if (mod) {
				data[l++] = offset;
				if (mod > 1) {
					data[l++] = offset >> 8;
					data[l++] = offset >> 16;
					data[l++] = offset >> 24;
				}
			}
		} else {
			offset = op->operands[1].offset * op->operands[1].offset_sign;
			data[l++] = 0x05;
			data[l++] = offset;
			data[l++] = offset >> 8;
			data[l++] = offset >> 16;
			data[l++] = offset >> 24;
		}
	}
	return l;
}
