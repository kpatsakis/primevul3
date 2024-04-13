static int cx24116_read_ucblocks(struct dvb_frontend *fe, u32 *ucblocks)
{
	struct cx24116_state *state = fe->demodulator_priv;

	dprintk("%s()\n", __func__);

	*ucblocks = (cx24116_readreg(state, CX24116_REG_UCB8) << 8) |
		cx24116_readreg(state, CX24116_REG_UCB0);

	return 0;
}
