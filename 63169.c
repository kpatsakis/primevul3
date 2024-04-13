static int opffree(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	switch (op->operands_count) {
	case 1:
		if (op->operands[0].type & OT_FPUREG & ~OT_REGALL)  {
			data[l++] = 0xdd;
			data[l++] = 0xc0 | op->operands[0].reg;
		} else {
			return -1;
		}
		break;
	default:
		return -1;
	}
	return l;
}
