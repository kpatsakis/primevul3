DLLIMPORT char *cfg_opt_getnstr(cfg_opt_t *opt, unsigned int index)
{
	if (!opt || opt->type != CFGT_STR) {
		errno = EINVAL;
		return NULL;
	}

	if (opt->values && index < opt->nvalues)
		return opt->values[index]->string;
	if (opt->simple_value.string)
		return *opt->simple_value.string;

	return NULL;
}