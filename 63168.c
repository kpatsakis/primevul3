static int opfdivrp(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	switch (op->operands_count) {
	case 0:
		data[l++] = 0xde;
		data[l++] = 0xf1;
		break;
	case 2:
		if ( op->operands[0].type & OT_FPUREG & ~OT_REGALL &&
		     op->operands[1].type & OT_FPUREG & ~OT_REGALL && op->operands[1].reg == 0 ) {
			data[l++] = 0xde;
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
