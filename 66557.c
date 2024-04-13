static void _6502_anal_esil_flags(RAnalOp *op, ut8 data0) {
	int enabled=0;
	char flag ='u';
	switch(data0) {
	case 0x78: // sei
		enabled = 1;
		flag = 'I';
		break;
	case 0x58: // cli
		enabled = 0;
		flag = 'I';
		break;
	case 0x38: // sec
		enabled = 1;
		flag = 'C';
		break;
	case 0x18: // clc
		enabled = 0;
		flag = 'C';
		break;
	case 0xf8: // sed
		enabled = 1;
		flag = 'D';
		break;
	case 0xd8: // cld
		enabled = 0;
		flag = 'D';
		break;
	case 0xb8: // clv
		enabled = 0;
		flag = 'V';
		break;
		break;
	}
	r_strbuf_setf (&op->esil, "%d,%c,=", enabled, flag);
}
