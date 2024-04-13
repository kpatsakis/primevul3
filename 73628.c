static int cx24116_set_fec(struct cx24116_state *state,
	fe_delivery_system_t delsys, fe_modulation_t mod, fe_code_rate_t fec)
{
	int ret = 0;

	dprintk("%s(0x%02x,0x%02x)\n", __func__, mod, fec);

	ret = cx24116_lookup_fecmod(state, delsys, mod, fec);

	if (ret < 0)
		return ret;

	state->dnxt.fec = fec;
	state->dnxt.fec_val = CX24116_MODFEC_MODES[ret].val;
	state->dnxt.fec_mask = CX24116_MODFEC_MODES[ret].mask;
	dprintk("%s() mask/val = 0x%02x/0x%02x\n", __func__,
		state->dnxt.fec_mask, state->dnxt.fec_val);

	return 0;
}
