static int cx24116_writereg(struct cx24116_state *state, int reg, int data)
{
	u8 buf[] = { reg, data };
	struct i2c_msg msg = { .addr = state->config->demod_address,
		.flags = 0, .buf = buf, .len = 2 };
	int err;

	if (debug > 1)
		printk("cx24116: %s: write reg 0x%02x, value 0x%02x\n",
			__func__, reg, data);

	err = i2c_transfer(state->i2c, &msg, 1);
	if (err != 1) {
		printk(KERN_ERR "%s: writereg error(err == %i, reg == 0x%02x,"
			 " value == 0x%02x)\n", __func__, err, reg, data);
		return -EREMOTEIO;
	}

	return 0;
}
