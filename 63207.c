static int opmovx(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	int word = 0;
	char *movx = op->mnemonic + 3;

	if (!(op->operands[0].type & OT_REGTYPE && op->operands[1].type & OT_MEMORY)) {
		return -1;
	}
	if (op->operands[1].type & OT_WORD) {
		word = 1;
	}

	data[l++] = 0x0f;
	if (!strcmp (movx, "zx")) {
		data[l++] = 0xb6 + word;
	} else if (!strcmp (movx, "sx")) {
		data[l++] = 0xbe + word;
	}
	data[l++] = op->operands[0].reg << 3 | op->operands[1].regs[0];
	if (op->operands[1].regs[0] == X86R_ESP) {
		data[l++] = 0x24;
	}

	return l;
}
