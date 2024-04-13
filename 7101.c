DLLIMPORT int cfg_opt_setnstr(cfg_opt_t *opt, const char *value, unsigned int index)
{
	char *newstr, *oldstr = NULL;
	cfg_value_t *val;

	if (!opt || opt->type != CFGT_STR) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	val = cfg_opt_getval(opt, index);
	if (!val)
		return CFG_FAIL;

	if (val->string)
		oldstr = val->string;

	if (value) {
		newstr = strdup(value);
		if (!newstr)
			return CFG_FAIL;
		val->string = newstr;
	} else {
		val->string = NULL;
	}

	if (oldstr)
		free(oldstr);
	opt->flags |= CFGF_MODIFIED;

	return CFG_SUCCESS;
}