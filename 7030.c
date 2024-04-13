DLLIMPORT int cfg_opt_rmnsec(cfg_opt_t *opt, unsigned int index)
{
	unsigned int n;
	cfg_value_t *val;

	if (!opt || opt->type != CFGT_SEC) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	n = cfg_opt_size(opt);
	if (index >= n)
		return CFG_FAIL;

	val = cfg_opt_getval(opt, index);
	if (!val)
		return CFG_FAIL;

	if (index + 1 != n) {
		/* not removing last, move the tail */
		memmove(&opt->values[index], &opt->values[index + 1], sizeof(opt->values[index]) * (n - index - 1));
	}
	--opt->nvalues;

	cfg_free(val->section);
	free(val);

	return CFG_SUCCESS;
}