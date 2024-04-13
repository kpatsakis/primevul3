DLLIMPORT unsigned int cfg_opt_size(cfg_opt_t *opt)
{
	if (opt)
		return opt->nvalues;
	return 0;
}