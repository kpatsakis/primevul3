static int cx24116_read_ber(struct dvb_frontend *fe, u32 *ber)
{
	struct cx24116_state *state = fe->demodulator_priv;

	dprintk("%s()\n", __func__);

	*ber =  (cx24116_readreg(state, CX24116_REG_BER24) << 24) |
		(cx24116_readreg(state, CX24116_REG_BER16) << 16) |
		(cx24116_readreg(state, CX24116_REG_BER8)  << 8)  |
		 cx24116_readreg(state, CX24116_REG_BER0);

	return 0;
}
