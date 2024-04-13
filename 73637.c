static int cx24116_writeregN(struct cx24116_state *state, int reg,
			     const u8 *data, u16 len)
{
	int ret = -EREMOTEIO;
	struct i2c_msg msg;
	u8 *buf;

	buf = kmalloc(len + 1, GFP_KERNEL);
	if (buf == NULL) {
		printk("Unable to kmalloc\n");
		ret = -ENOMEM;
		goto error;
	}

	*(buf) = reg;
	memcpy(buf + 1, data, len);

	msg.addr = state->config->demod_address;
	msg.flags = 0;
	msg.buf = buf;
	msg.len = len + 1;

	if (debug > 1)
		printk(KERN_INFO "cx24116: %s:  write regN 0x%02x, len = %d\n",
			__func__, reg, len);

	ret = i2c_transfer(state->i2c, &msg, 1);
	if (ret != 1) {
		printk(KERN_ERR "%s: writereg error(err == %i, reg == 0x%02x\n",
			 __func__, ret, reg);
		ret = -EREMOTEIO;
	}

error:
	kfree(buf);

	return ret;
}
