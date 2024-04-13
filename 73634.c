static int cx24116_sleep(struct dvb_frontend *fe)
{
	struct cx24116_state *state = fe->demodulator_priv;
	struct cx24116_cmd cmd;
	int ret;

	dprintk("%s()\n", __func__);

	/* Firmware CMD 36: Power config */
	cmd.args[0x00] = CMD_TUNERSLEEP;
	cmd.args[0x01] = 1;
	cmd.len = 0x02;
	ret = cx24116_cmd_execute(fe, &cmd);
	if (ret != 0)
		return ret;

	/* Power off (Shutdown clocks) */
	cx24116_writereg(state, 0xea, 0xff);
	cx24116_writereg(state, 0xe1, 1);
	cx24116_writereg(state, 0xe0, 1);

	return 0;
}
