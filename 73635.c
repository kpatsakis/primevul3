static int cx24116_wait_for_lnb(struct dvb_frontend *fe)
{
	struct cx24116_state *state = fe->demodulator_priv;
	int i;

	dprintk("%s() qstatus = 0x%02x\n", __func__,
		cx24116_readreg(state, CX24116_REG_QSTATUS));

	/* Wait for up to 300 ms */
	for (i = 0; i < 30 ; i++) {
		if (cx24116_readreg(state, CX24116_REG_QSTATUS) & 0x20)
			return 0;
		msleep(10);
	}

	dprintk("%s(): LNB not ready\n", __func__);

	return -ETIMEDOUT; /* -EBUSY ? */
}
