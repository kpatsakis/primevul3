DLLIMPORT char *cfg_opt_getcomment(cfg_opt_t *opt)
{
	if (opt)
		return opt->comment;

	return NULL;
}