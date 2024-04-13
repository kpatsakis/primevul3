DLLIMPORT double cfg_opt_getnfloat(cfg_opt_t *opt, unsigned int index)
{
	if (!opt || opt->type != CFGT_FLOAT) {
		errno = EINVAL;
		return 0;
	}

	if (opt->values && index < opt->nvalues)
		return opt->values[index]->fpnumber;
	if (opt->simple_value.fpnumber)
		return *opt->simple_value.fpnumber;

	return 0;
}