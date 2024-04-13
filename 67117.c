INST_HANDLER (lac) {	// LAC Z, Rd
	int d = ((buf[0] >> 4) & 0xf) | ((buf[1] & 0x1) << 4);

	__generic_ld_st (op, "ram", 'z', 1, 0, 0, 0);	// 0: Read (RAMPZ:Z)
	ESIL_A ("r%d,0xff,^,&,", d);			// 0: (Z) & ~Rd
	ESIL_A ("DUP,r%d,=,", d);			// Rd = [0]
	__generic_ld_st (op, "ram", 'z', 1, 0, 0, 1);	// Store in RAM
}
