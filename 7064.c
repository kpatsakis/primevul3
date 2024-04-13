DLLIMPORT signed long cfg_opt_getnint(cfg_opt_t *opt, unsigned int index)
{
	if (!opt || opt->type != CFGT_INT) {
		errno = EINVAL;
		return 0;
	}

	if (opt->values && index < opt->nvalues)
		return opt->values[index]->number;
	if (opt->simple_value.number)
		return *opt->simple_value.number;

	return 0;
}