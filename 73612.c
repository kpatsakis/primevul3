static int cx24116_cmd_execute(struct dvb_frontend *fe, struct cx24116_cmd *cmd)
{
	struct cx24116_state *state = fe->demodulator_priv;
	int i, ret;

	dprintk("%s()\n", __func__);

	/* Load the firmware if required */
	ret = cx24116_firmware_ondemand(fe);
	if (ret != 0) {
		printk(KERN_ERR "%s(): Unable initialise the firmware\n",
			__func__);
		return ret;
	}

	/* Write the command */
	for (i = 0; i < cmd->len ; i++) {
		dprintk("%s: 0x%02x == 0x%02x\n", __func__, i, cmd->args[i]);
		cx24116_writereg(state, i, cmd->args[i]);
	}

	/* Start execution and wait for cmd to terminate */
	cx24116_writereg(state, CX24116_REG_EXECUTE, 0x01);
	while (cx24116_readreg(state, CX24116_REG_EXECUTE)) {
		msleep(10);
		if (i++ > 64) {
			/* Avoid looping forever if the firmware does
				not respond */
			printk(KERN_WARNING "%s() Firmware not responding\n",
				__func__);
			return -EREMOTEIO;
		}
	}
	return 0;
}
