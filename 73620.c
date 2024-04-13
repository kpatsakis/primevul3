static int cx24116_read_signal_strength(struct dvb_frontend *fe,
	u16 *signal_strength)
{
	struct cx24116_state *state = fe->demodulator_priv;
	struct cx24116_cmd cmd;
	int ret;
	u16 sig_reading;

	dprintk("%s()\n", __func__);

	/* Firmware CMD 19: Get AGC */
	cmd.args[0x00] = CMD_GETAGC;
	cmd.len = 0x01;
	ret = cx24116_cmd_execute(fe, &cmd);
	if (ret != 0)
		return ret;

	sig_reading =
		(cx24116_readreg(state,
			CX24116_REG_SSTATUS) & CX24116_SIGNAL_MASK) |
		(cx24116_readreg(state, CX24116_REG_SIGNAL) << 6);
	*signal_strength = 0 - sig_reading;

	dprintk("%s: raw / cooked = 0x%04x / 0x%04x\n",
		__func__, sig_reading, *signal_strength);

	return 0;
}
