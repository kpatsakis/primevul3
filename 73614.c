static int cx24116_diseqc_send_burst(struct dvb_frontend *fe,
	fe_sec_mini_cmd_t burst)
{
	struct cx24116_state *state = fe->demodulator_priv;
	int ret;

	dprintk("%s(%d) toneburst=%d\n", __func__, burst, toneburst);

	/* DiSEqC burst */
	if (burst == SEC_MINI_A)
		state->dsec_cmd.args[CX24116_DISEQC_BURST] =
			CX24116_DISEQC_MINI_A;
	else if (burst == SEC_MINI_B)
		state->dsec_cmd.args[CX24116_DISEQC_BURST] =
			CX24116_DISEQC_MINI_B;
	else
		return -EINVAL;

	/* DiSEqC toneburst */
	if (toneburst != CX24116_DISEQC_MESGCACHE)
		/* Burst is cached */
		return 0;

	/* Burst is to be sent with cached message */

	/* Wait for LNB ready */
	ret = cx24116_wait_for_lnb(fe);
	if (ret != 0)
		return ret;

	/* Wait for voltage/min repeat delay */
	msleep(100);

	/* Command */
	ret = cx24116_cmd_execute(fe, &state->dsec_cmd);
	if (ret != 0)
		return ret;

	/*
	 * Wait for send
	 *
	 * Eutelsat spec:
	 * >15ms delay          + (XXX determine if FW does this, see set_tone)
	 *  13.5ms per byte     +
	 * >15ms delay          +
	 *  12.5ms burst        +
	 * >15ms delay            (XXX determine if FW does this, see set_tone)
	 */
	msleep((state->dsec_cmd.args[CX24116_DISEQC_MSGLEN] << 4) + 60);

	return 0;
}
