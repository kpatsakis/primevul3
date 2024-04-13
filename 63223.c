static int optest(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	if (!op->operands[0].type || !op->operands[1].type) {
		eprintf ("Error: Invalid operands\n");
		return -1;
	}
	if (a->bits == 64) {
		if (op->operands[0].type & OT_MEMORY ||
			op->operands[1].type & OT_MEMORY) {
			data[l++] = 0x67;
		}
		if (op->operands[0].type & OT_QWORD &&
			op->operands[1].type & OT_QWORD) {
			if (op->operands[0].extended &&
			    op->operands[1].extended) {
					data[l++] = 0x4d;
				} else {
					data[l++] = 0x48;
				}
		}
	}

	if (op->operands[1].type & OT_CONSTANT) {
		if (op->operands[0].type & OT_BYTE) {
			data[l++] = 0xf6;
			data[l++] = op->operands[0].regs[0];
			data[l++] = op->operands[1].immediate;
			return l;
		}
		data[l++] = 0xf7;
		if (op->operands[0].type & OT_MEMORY) {
			data[l++] = 0x00 | op->operands[0].regs[0];
		} else {
			data[l++] = 0xc0 | op->operands[0].reg;
		}
		data[l++] = op->operands[1].immediate >> 0;
		data[l++] = op->operands[1].immediate >> 8;
		data[l++] = op->operands[1].immediate >> 16;
		data[l++] = op->operands[1].immediate >> 24;
		return l;
	}
	if (op->operands[0].type & OT_BYTE ||
		op->operands[1].type & OT_BYTE) {
		data[l++] = 0x84;
	} else {
		data[l++] = 0x85;
	}
	if (op->operands[0].type & OT_MEMORY) {
		data[l++] = 0x00 | op->operands[1].reg << 3 | op->operands[0].regs[0];
	} else {
		if (op->operands[1].type & OT_MEMORY) {
			data[l++] = 0x00 | op->operands[0].reg << 3 | op->operands[1].regs[0];
		} else {
			data[l++] = 0xc0 | op->operands[1].reg << 3 | op->operands[0].reg;
		}
	}
	return l;
}
