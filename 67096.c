INST_HANDLER (bclr) {	// BCLR s
	int s = (buf[0] >> 4) & 0x7;
	ESIL_A ("0xff,%d,1,<<,^,sreg,&=,", s);
}
