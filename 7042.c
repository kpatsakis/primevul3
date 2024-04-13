DLLIMPORT int cfg_opt_setnint(cfg_opt_t *opt, long int value, unsigned int index)
{
	cfg_value_t *val;

	if (!opt || opt->type != CFGT_INT) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	val = cfg_opt_getval(opt, index);
	if (!val)
		return CFG_FAIL;

	val->number = value;
	opt->flags |= CFGF_MODIFIED;

	return CFG_SUCCESS;
}