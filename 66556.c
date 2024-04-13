static void _6502_anal_esil_ccall(RAnalOp *op, ut8 data0)
{
	char *flag;
	switch(data0) {
	case 0x10: // bpl $ffff
		flag = "N,!";
		break;
	case 0x30: // bmi $ffff
		flag = "N";
		break;
	case 0x50: // bvc $ffff
		flag = "V,!";
		break;
	case 0x70: // bvs $ffff
		flag = "V";
		break;
	case 0x90: // bcc $ffff
		flag = "C,!";
		break;
	case 0xb0: // bcs $ffff
		flag = "C";
		break;
	case 0xd0: // bne $ffff
		flag = "Z,!";
		break;
	case 0xf0: // beq $ffff
		flag = "Z";
		break;
	default:
		flag = "unk";
		break;
	}
	r_strbuf_setf (&op->esil, "%s,?{,0x%04x,pc,=,}", flag, (op->jump & 0xffff));
}
