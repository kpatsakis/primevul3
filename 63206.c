static int oploop(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	data[l++] = 0xe2;
	st8 delta = op->operands[0].immediate - a->pc - 2;
	data[l++] = (ut8)delta;
	return l;
}
