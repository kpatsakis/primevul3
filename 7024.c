static cfg_value_t *cfg_addval(cfg_opt_t *opt)
{
	void *ptr;

	ptr = realloc(opt->values, (opt->nvalues + 1) * sizeof(cfg_value_t *));
	if (!ptr)
		return NULL;

	opt->values = ptr;
	opt->values[opt->nvalues] = calloc(1, sizeof(cfg_value_t));
	if (!opt->values[opt->nvalues])
		return NULL;

	opt->flags |= CFGF_MODIFIED;

	return opt->values[opt->nvalues++];
}