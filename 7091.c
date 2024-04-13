DLLIMPORT cfg_bool_t cfg_opt_getnbool(cfg_opt_t *opt, unsigned int index)
{
	if (!opt || opt->type != CFGT_BOOL) {
		errno = EINVAL;
		return cfg_false;
	}

	if (opt->values && index < opt->nvalues)
		return opt->values[index]->boolean;
	if (opt->simple_value.boolean)
		return *opt->simple_value.boolean;

	return cfg_false;
}