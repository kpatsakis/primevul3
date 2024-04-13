static ut8 getsib(const ut8 sib) {
	if (!sib) {
		return 0;
	}
	return (sib & 0x8) ? 3 : getsib ((sib << 1) | 1) - 1;
}
