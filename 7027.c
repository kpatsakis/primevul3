DLLIMPORT int cfg_opt_setmulti(cfg_t *cfg, cfg_opt_t *opt, unsigned int nvalues, char **values)
{
	cfg_opt_t old;
	unsigned int i;

	if (!opt || !nvalues) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	old = *opt;
	opt->nvalues = 0;
	opt->values = NULL;

	for (i = 0; i < nvalues; i++) {
		if (cfg_setopt(cfg, opt, values[i]))
			continue;

		/* ouch, revert */
		cfg_free_value(opt);
		opt->nvalues = old.nvalues;
		opt->values = old.values;
		opt->flags &= ~(CFGF_RESET | CFGF_MODIFIED);
		opt->flags |= old.flags & (CFGF_RESET | CFGF_MODIFIED);

		return CFG_FAIL;
	}

	cfg_free_value(&old);
	opt->flags |= CFGF_MODIFIED;

	return CFG_SUCCESS;
}