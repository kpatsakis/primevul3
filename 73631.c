static int cx24116_set_symbolrate(struct cx24116_state *state, u32 rate)
{
	dprintk("%s(%d)\n", __func__, rate);

	/*  check if symbol rate is within limits */
	if ((rate > state->frontend.ops.info.symbol_rate_max) ||
	    (rate < state->frontend.ops.info.symbol_rate_min)) {
		dprintk("%s() unsupported symbol_rate = %d\n", __func__, rate);
		return -EOPNOTSUPP;
	}

	state->dnxt.symbol_rate = rate;
	dprintk("%s() symbol_rate = %d\n", __func__, rate);

	return 0;
}
