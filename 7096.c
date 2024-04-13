DLLIMPORT int cfg_setnstr(cfg_t *cfg, const char *name, const char *value, unsigned int index)
{
	cfg_opt_t *opt;

	opt = cfg_getopt(cfg, name);
	if (opt && opt->validcb2 && (*opt->validcb2)(cfg, opt, (void *)value) != 0)
		return CFG_FAIL;

	return cfg_opt_setnstr(opt, value, index);
}