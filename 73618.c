static int cx24116_lookup_fecmod(struct cx24116_state *state,
	fe_delivery_system_t d, fe_modulation_t m, fe_code_rate_t f)
{
	int i, ret = -EOPNOTSUPP;

	dprintk("%s(0x%02x,0x%02x)\n", __func__, m, f);

	for (i = 0; i < ARRAY_SIZE(CX24116_MODFEC_MODES); i++) {
		if ((d == CX24116_MODFEC_MODES[i].delivery_system) &&
			(m == CX24116_MODFEC_MODES[i].modulation) &&
			(f == CX24116_MODFEC_MODES[i].fec)) {
				ret = i;
				break;
			}
	}

	return ret;
}
