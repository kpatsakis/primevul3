static int opsmsw(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	switch (op->operands_count) {
	case 1:
		if ( a->bits == 64 ) {
			data[l++] = 0x48;
		}
		data[l++] = 0x0f;
		data[l++] = 0x01;
		if ( op->operands[0].type & OT_MEMORY ) {
			data[l++] = 0x20 | op->operands[0].regs[0];
		} else {
			data[l++] = 0xe0 | op->operands[0].reg;
		}
		break;
	default:
		return -1;
	}
	return l;
}
