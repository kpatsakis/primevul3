static int opstos(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	if (!strcmp(op->mnemonic, "stosw")) {
		data[l++] = 0x66;
	}
	if (!strcmp(op->mnemonic, "stosb")) {
		data[l++] = 0xaa;
	} else if (!strcmp(op->mnemonic, "stosw")) {
		data[l++] = 0xab;
	} else if (!strcmp(op->mnemonic, "stosd")) {
		data[l++] = 0xab;
	}
	return l;
}
