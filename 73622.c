static int cx24116_read_snr_esno(struct dvb_frontend *fe, u16 *snr)
{
	struct cx24116_state *state = fe->demodulator_priv;

	dprintk("%s()\n", __func__);

	*snr = cx24116_readreg(state, CX24116_REG_QUALITY8) << 8 |
		cx24116_readreg(state, CX24116_REG_QUALITY0);

	dprintk("%s: raw 0x%04x\n", __func__, *snr);

	return 0;
}
