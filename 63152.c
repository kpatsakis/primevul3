static int opaam(RAsm *a, ut8 *data, const Opcode *op) {
	int l = 0;
	int immediate = op->operands[0].immediate * op->operands[0].sign;
	data[l++] = 0xd4;
	if (immediate == 0) {
		data[l++] = 0x0a;
	} else if (immediate < 256 && immediate > -129) {
		data[l++] = immediate;
	}
	return l;
}
