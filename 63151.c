static int is_al_reg(const Operand *op) {
	if (op->type & OT_MEMORY) {
		return 0;
	}
	if (op->reg == X86R_AL && op->type & OT_BYTE) {
		return 1;
	}
	return 0;
}
