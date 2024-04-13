DLLIMPORT int cfg_opt_setnbool(cfg_opt_t *opt, cfg_bool_t value, unsigned int index)
{
	cfg_value_t *val;

	if (!opt || opt->type != CFGT_BOOL) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	val = cfg_opt_getval(opt, index);
	if (!val)
		return CFG_FAIL;

	val->boolean = value;
	opt->flags |= CFGF_MODIFIED;

	return CFG_SUCCESS;
}