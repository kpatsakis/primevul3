static int oplea(RAsm *a, ut8 *data, const Opcode *op){
	int l = 0;
	int mod = 0;
	st32 offset = 0;
	int reg = 0;
	int rm = 0;
	if (op->operands[0].type & OT_REGALL &&
	    op->operands[1].type & (OT_MEMORY | OT_CONSTANT)) {
		if (a->bits == 64) {
			data[l++] = 0x48;
		}
		data[l++] = 0x8d;
		if (op->operands[1].regs[0] == X86R_UNDEFINED) {
			int high = 0xff00 & op->operands[1].offset;
			data[l++] = op->operands[0].reg << 3 | 5;
			data[l++] = op->operands[1].offset;
			data[l++] = high >> 8;
			data[l++] = op->operands[1].offset >> 16;
			data[l++] = op->operands[1].offset >> 24;
			return l;
		} else {
			reg = op->operands[0].reg;
			rm = op->operands[1].regs[0];

			offset = op->operands[1].offset * op->operands[1].offset_sign;
			if (offset != 0 || op->operands[1].regs[0] == X86R_EBP) {
				mod = 1;
				if (offset >= 128 || offset < -128) {
					mod = 2;
				}
				data[l++] = mod << 6 | reg << 3 | rm;
				if (op->operands[1].regs[0] == X86R_ESP) {
					data[l++] = 0x24;
				}
				data[l++] = offset;
				if (mod == 2) {
					data[l++] = offset >> 8;
					data[l++] = offset >> 16;
					data[l++] = offset >> 24;
				}
			} else {
				data[l++] = op->operands[0].reg << 3 | op->operands[1].regs[0];
				if (op->operands[1].regs[0] == X86R_ESP) {
					data[l++] = 0x24;
				}
			}

		}
	}
	return l;
}
