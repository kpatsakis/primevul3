static int opcdqe(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	if (a->bits == 64) {
		data[l++] = 0x48;
	}
	data[l++] = 0x98;
	return l;
}
