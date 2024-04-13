static int opfdivr(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	switch (op->operands_count) {
	case 1:
		if ( op->operands[0].type & OT_MEMORY ) {
			if ( op->operands[0].type & OT_DWORD ) {
				data[l++] = 0xd8;
				data[l++] = 0x38 | op->operands[0].regs[0];
			} else if ( op->operands[0].type & OT_QWORD ) {
				data[l++] = 0xdc;
				data[l++] = 0x38 | op->operands[0].regs[0];
			} else {
				return -1;
			}
		} else {
			return -1;
		}
		break;
	case 2:
		if ( op->operands[0].type & OT_FPUREG & ~OT_REGALL && op->operands[0].reg == 0 &&
		     op->operands[1].type & OT_FPUREG & ~OT_REGALL ) {
			data[l++] = 0xd8;
			data[l++] = 0xf8 | op->operands[1].reg;
		} else if ( op->operands[0].type & OT_FPUREG & ~OT_REGALL &&
			    op->operands[1].type & OT_FPUREG & ~OT_REGALL && op->operands[1].reg == 0 ) {
			data[l++] = 0xdc;
			data[l++] = 0xf0 | op->operands[0].reg;
		} else {
			return -1;
		}
		break;
	default:
		return -1;
	}
	return l;
}
