static void _6502_anal_esil_pop(RAnalOp *op, ut8 data0) {
	char *reg = (data0==0x28) ? "flags" : "a";
	r_strbuf_setf (&op->esil, "sp,++=,sp,0x100,+,[1],%s,=", reg);

	if (data0==0x68) _6502_anal_update_flags (op, _6502_FLAGS_NZ);
}
