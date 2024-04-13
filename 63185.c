static int opfnstenv(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	switch (op->operands_count) {
	case 1:
		if ( op->operands[0].type & OT_MEMORY ) {
			data[l++] = 0xd9;
			data[l++] = 0x30 | op->operands[0].regs[0];
		} else {
			return -1;
		}
		break;
	default:
		return -1;
	}
	return l;
}
