DLLIMPORT int cfg_opt_setnfloat(cfg_opt_t *opt, double value, unsigned int index)
{
	cfg_value_t *val;

	if (!opt || opt->type != CFGT_FLOAT) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	val = cfg_opt_getval(opt, index);
	if (!val)
		return CFG_FAIL;

	val->fpnumber = value;
	opt->flags |= CFGF_MODIFIED;

	return CFG_SUCCESS;
}