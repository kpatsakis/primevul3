static int opstr(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	switch (op->operands_count) {
	case 1:
		if ( op->operands[0].type & OT_MEMORY &&
		     op->operands[0].type & OT_WORD ) {
			data[l++] = 0x0f;
			data[l++] = 0x00;
			data[l++] = 0x08 | op->operands[0].regs[0];
		} else if ( op->operands[0].type & OT_GPREG &&
			    op->operands[0].type & OT_DWORD ) {
			data[l++] = 0x0f;
			data[l++] = 0x00;
			data[l++] = 0xc8 | op->operands[0].reg;
		} else {
			return -1;
		}
		break;
	default:
		return -1;
	}
	return l;
}
