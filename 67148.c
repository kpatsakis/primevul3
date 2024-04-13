INST_HANDLER (st) {	// ST X, Rr
	ESIL_A ("r%d,", ((buf[1] & 1) << 4) | ((buf[0] >> 4) & 0xf));
	__generic_ld_st (
		op, "ram",
		'x',				// use index register X
		0,				// no use RAMP* registers
		(buf[0] & 0xf) == 0xe
			? -1			// pre decremented
			: (buf[0] & 0xf) == 0xd
				? 1		// post increment
				: 0,		// no increment
		0,				// offset always 0
		1);				// store operation (st)
}
