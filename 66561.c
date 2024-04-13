static void _6502_anal_esil_inc_reg(RAnalOp *op, ut8 data0, char* sign)
{
	char* reg = NULL;

	switch(data0) {
	case 0xe8: // inx
	case 0xca: // dex
		reg = "x";
		break;
	case 0xc8: // iny
	case 0x88: // dey
		reg = "y";
		break;
	}
	r_strbuf_setf (&op->esil, "%s,%s%s=", reg, sign, sign);
	_6502_anal_update_flags (op, _6502_FLAGS_NZ);
}
