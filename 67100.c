INST_HANDLER (cbi) {	// CBI A, b
	int a = (buf[0] >> 3) & 0x1f;
	int b = buf[0] & 0x07;
	RStrBuf *io_port;

	op->family = R_ANAL_OP_FAMILY_IO;
	op->type2 = 1;
	op->val = a;

	io_port = __generic_io_dest (a, 0, cpu);
	ESIL_A ("0xff,%d,1,<<,^,%s,&,", b, io_port);
	r_strbuf_free (io_port);

	io_port = __generic_io_dest (a, 1, cpu);
	ESIL_A ("%s,", r_strbuf_get (io_port));
	r_strbuf_free (io_port);
}
