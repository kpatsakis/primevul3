DLLIMPORT cfg_validate_callback2_t cfg_set_validate_func2(cfg_t *cfg, const char *name, cfg_validate_callback2_t vf)
{
	cfg_opt_t *opt;
	cfg_validate_callback2_t oldvf;

	opt = cfg_getopt_array(cfg->opts, cfg->flags, name);
	if (!opt)
		return NULL;

	oldvf = opt->validcb2;
	opt->validcb2 = vf;

	return oldvf;
}