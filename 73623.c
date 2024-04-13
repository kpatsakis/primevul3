static int cx24116_read_snr_pct(struct dvb_frontend *fe, u16 *snr)
{
	struct cx24116_state *state = fe->demodulator_priv;
	u8 snr_reading;
	static const u32 snr_tab[] = { /* 10 x Table (rounded up) */
		0x00000, 0x0199A, 0x03333, 0x04ccD, 0x06667,
		0x08000, 0x0999A, 0x0b333, 0x0cccD, 0x0e667,
		0x10000, 0x1199A, 0x13333, 0x14ccD, 0x16667,
		0x18000 };

	dprintk("%s()\n", __func__);

	snr_reading = cx24116_readreg(state, CX24116_REG_QUALITY0);

	if (snr_reading >= 0xa0 /* 100% */)
		*snr = 0xffff;
	else
		*snr = snr_tab[(snr_reading & 0xf0) >> 4] +
			(snr_tab[(snr_reading & 0x0f)] >> 4);

	dprintk("%s: raw / cooked = 0x%02x / 0x%04x\n", __func__,
		snr_reading, *snr);

	return 0;
}
