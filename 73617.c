static int cx24116_load_firmware(struct dvb_frontend *fe,
	const struct firmware *fw)
{
	struct cx24116_state *state = fe->demodulator_priv;
	struct cx24116_cmd cmd;
	int i, ret, len, max, remaining;
	unsigned char vers[4];

	dprintk("%s\n", __func__);
	dprintk("Firmware is %zu bytes (%02x %02x .. %02x %02x)\n",
			fw->size,
			fw->data[0],
			fw->data[1],
			fw->data[fw->size-2],
			fw->data[fw->size-1]);

	/* Toggle 88x SRST pin to reset demod */
	if (state->config->reset_device)
		state->config->reset_device(fe);

	/* Begin the firmware load process */
	/* Prepare the demod, load the firmware, cleanup after load */

	/* Init PLL */
	cx24116_writereg(state, 0xE5, 0x00);
	cx24116_writereg(state, 0xF1, 0x08);
	cx24116_writereg(state, 0xF2, 0x13);

	/* Start PLL */
	cx24116_writereg(state, 0xe0, 0x03);
	cx24116_writereg(state, 0xe0, 0x00);

	/* Unknown */
	cx24116_writereg(state, CX24116_REG_CLKDIV, 0x46);
	cx24116_writereg(state, CX24116_REG_RATEDIV, 0x00);

	/* Unknown */
	cx24116_writereg(state, 0xF0, 0x03);
	cx24116_writereg(state, 0xF4, 0x81);
	cx24116_writereg(state, 0xF5, 0x00);
	cx24116_writereg(state, 0xF6, 0x00);

	/* Split firmware to the max I2C write len and write.
	 * Writes whole firmware as one write when i2c_wr_max is set to 0. */
	if (state->config->i2c_wr_max)
		max = state->config->i2c_wr_max;
	else
		max = INT_MAX; /* enough for 32k firmware */

	for (remaining = fw->size; remaining > 0; remaining -= max - 1) {
		len = remaining;
		if (len > max - 1)
			len = max - 1;

		cx24116_writeregN(state, 0xF7, &fw->data[fw->size - remaining],
			len);
	}

	cx24116_writereg(state, 0xF4, 0x10);
	cx24116_writereg(state, 0xF0, 0x00);
	cx24116_writereg(state, 0xF8, 0x06);

	/* Firmware CMD 10: VCO config */
	cmd.args[0x00] = CMD_SET_VCO;
	cmd.args[0x01] = 0x05;
	cmd.args[0x02] = 0xdc;
	cmd.args[0x03] = 0xda;
	cmd.args[0x04] = 0xae;
	cmd.args[0x05] = 0xaa;
	cmd.args[0x06] = 0x04;
	cmd.args[0x07] = 0x9d;
	cmd.args[0x08] = 0xfc;
	cmd.args[0x09] = 0x06;
	cmd.len = 0x0a;
	ret = cx24116_cmd_execute(fe, &cmd);
	if (ret != 0)
		return ret;

	cx24116_writereg(state, CX24116_REG_SSTATUS, 0x00);

	/* Firmware CMD 14: Tuner config */
	cmd.args[0x00] = CMD_TUNERINIT;
	cmd.args[0x01] = 0x00;
	cmd.args[0x02] = 0x00;
	cmd.len = 0x03;
	ret = cx24116_cmd_execute(fe, &cmd);
	if (ret != 0)
		return ret;

	cx24116_writereg(state, 0xe5, 0x00);

	/* Firmware CMD 13: MPEG config */
	cmd.args[0x00] = CMD_MPEGCONFIG;
	cmd.args[0x01] = 0x01;
	cmd.args[0x02] = 0x75;
	cmd.args[0x03] = 0x00;
	if (state->config->mpg_clk_pos_pol)
		cmd.args[0x04] = state->config->mpg_clk_pos_pol;
	else
		cmd.args[0x04] = 0x02;
	cmd.args[0x05] = 0x00;
	cmd.len = 0x06;
	ret = cx24116_cmd_execute(fe, &cmd);
	if (ret != 0)
		return ret;

	/* Firmware CMD 35: Get firmware version */
	cmd.args[0x00] = CMD_UPDFWVERS;
	cmd.len = 0x02;
	for (i = 0; i < 4; i++) {
		cmd.args[0x01] = i;
		ret = cx24116_cmd_execute(fe, &cmd);
		if (ret != 0)
			return ret;
		vers[i] = cx24116_readreg(state, CX24116_REG_MAILBOX);
	}
	printk(KERN_INFO "%s: FW version %i.%i.%i.%i\n", __func__,
		vers[0], vers[1], vers[2], vers[3]);

	return 0;
}
