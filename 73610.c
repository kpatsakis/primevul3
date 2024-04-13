struct dvb_frontend *cx24116_attach(const struct cx24116_config *config,
	struct i2c_adapter *i2c)
{
	struct cx24116_state *state = NULL;
	int ret;

	dprintk("%s\n", __func__);

	/* allocate memory for the internal state */
	state = kzalloc(sizeof(struct cx24116_state), GFP_KERNEL);
	if (state == NULL)
		goto error1;

	state->config = config;
	state->i2c = i2c;

	/* check if the demod is present */
	ret = (cx24116_readreg(state, 0xFF) << 8) |
		cx24116_readreg(state, 0xFE);
	if (ret != 0x0501) {
		printk(KERN_INFO "Invalid probe, probably not a CX24116 device\n");
		goto error2;
	}

	/* create dvb_frontend */
	memcpy(&state->frontend.ops, &cx24116_ops,
		sizeof(struct dvb_frontend_ops));
	state->frontend.demodulator_priv = state;
	return &state->frontend;

error2: kfree(state);
error1: return NULL;
}
