DLLIMPORT cfg_validate_callback_t cfg_set_validate_func(cfg_t *cfg, const char *name, cfg_validate_callback_t vf)
{
	cfg_opt_t *opt;
	cfg_validate_callback_t oldvf;

	opt = cfg_getopt_array(cfg->opts, cfg->flags, name);
	if (!opt)
		return NULL;

	oldvf = opt->validcb;
	opt->validcb = vf;

	return oldvf;
}