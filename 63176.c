static int opfist(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	switch (op->operands_count) {
	case 1:
		if ( op->operands[0].type & OT_MEMORY ) {
			if ( op->operands[0].type & OT_WORD ) {
				data[l++] = 0xdf;
				data[l++] = 0x10 | op->operands[0].regs[0];
			} else if ( op->operands[0].type & OT_DWORD ) {
				data[l++] = 0xdb;
				data[l++] = 0x10 | op->operands[0].regs[0];
			} else {
				return -1;
			}
		} else {
			return -1;
		}
		break;
	default:
		return -1;
	}
	return l;
}
