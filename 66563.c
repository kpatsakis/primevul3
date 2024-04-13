static void _6502_anal_esil_push(RAnalOp *op, ut8 data0) {
	char *reg = (data0==0x08) ? "flags" : "a";
	r_strbuf_setf (&op->esil, "%s,sp,0x100,+,=[1],sp,--=", reg);
}
