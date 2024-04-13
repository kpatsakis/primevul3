static cfg_value_t *cfg_opt_getval(cfg_opt_t *opt, unsigned int index)
{
	cfg_value_t *val = NULL;

	if (index != 0 && !is_set(CFGF_LIST, opt->flags) && !is_set(CFGF_MULTI, opt->flags)) {
		errno = EINVAL;
		return NULL;
	}

	if (opt->simple_value.ptr)
		val = (cfg_value_t *)opt->simple_value.ptr;
	else {
		if (is_set(CFGF_RESET, opt->flags)) {
			cfg_free_value(opt);
			opt->flags &= ~CFGF_RESET;
		}

		if (index >= opt->nvalues)
			val = cfg_addval(opt);
		else
			val = opt->values[index];
	}

	return val;
}