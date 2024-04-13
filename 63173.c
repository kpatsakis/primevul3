static int opfidivr(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	switch (op->operands_count) {
	case 1:
		if ( op->operands[0].type & OT_MEMORY ) {
			if ( op->operands[0].type & OT_DWORD ) {
				data[l++] = 0xda;
				data[l++] = 0x38 | op->operands[0].regs[0];
			} else if ( op->operands[0].type & OT_WORD ) {
				data[l++] = 0xde;
				data[l++] = 0x38 | op->operands[0].regs[0];
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
