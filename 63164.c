static int opfcmov(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	char* fcmov = op->mnemonic + strlen("fcmov");
	switch (op->operands_count) {
	case 2:
		if ( op->operands[0].type & OT_FPUREG & ~OT_REGALL && op->operands[0].reg == 0 &&
		     op->operands[1].type & OT_FPUREG & ~OT_REGALL ) {
			if ( !strcmp( fcmov, "b" ) ) {
				data[l++] = 0xda;
				data[l++] = 0xc0 | op->operands[1].reg;
			} else if ( !strcmp( fcmov, "e" ) ) {
				data[l++] = 0xda;
				data[l++] = 0xc8 | op->operands[1].reg;
			} else if ( !strcmp( fcmov, "be" ) ) {
				data[l++] = 0xda;
				data[l++] = 0xd0 | op->operands[1].reg;
			} else if ( !strcmp( fcmov, "u" ) ) {
				data[l++] = 0xda;
				data[l++] = 0xd8 | op->operands[1].reg;
			} else if ( !strcmp( fcmov, "nb" ) ) {
				data[l++] = 0xdb;
				data[l++] = 0xc0 | op->operands[1].reg;
			} else if ( !strcmp( fcmov, "ne" ) ) {
				data[l++] = 0xdb;
				data[l++] = 0xc8 | op->operands[1].reg;
			} else if ( !strcmp( fcmov, "nbe" ) ) {
				data[l++] = 0xdb;
				data[l++] = 0xd0 | op->operands[1].reg;
			} else if ( !strcmp( fcmov, "nu" ) ) {
				data[l++] = 0xdb;
				data[l++] = 0xd8 | op->operands[1].reg;
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
