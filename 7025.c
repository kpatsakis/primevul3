DLLIMPORT const char *cfg_opt_name(cfg_opt_t *opt)
{
	if (opt)
		return opt->name;
	return NULL;
}