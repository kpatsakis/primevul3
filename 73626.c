static int cx24116_readreg(struct cx24116_state *state, u8 reg)
{
	int ret;
	u8 b0[] = { reg };
	u8 b1[] = { 0 };
	struct i2c_msg msg[] = {
		{ .addr = state->config->demod_address, .flags = 0,
			.buf = b0, .len = 1 },
		{ .addr = state->config->demod_address, .flags = I2C_M_RD,
			.buf = b1, .len = 1 }
	};

	ret = i2c_transfer(state->i2c, msg, 2);

	if (ret != 2) {
		printk(KERN_ERR "%s: reg=0x%x (error=%d)\n",
			__func__, reg, ret);
		return ret;
	}

	if (debug > 1)
		printk(KERN_INFO "cx24116: read reg 0x%02x, value 0x%02x\n",
			reg, b1[0]);

	return b1[0];
}
