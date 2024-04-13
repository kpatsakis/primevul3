static int fpu_insn(RAnal* anal, RAnalOp* op, ut16 code){
	op->family = R_ANAL_OP_FAMILY_FPU;
	return op->size;
}
