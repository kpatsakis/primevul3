DLLIMPORT int cfg_setnint(cfg_t *cfg, const char *name, long int value, unsigned int index)
{
	cfg_opt_t *opt;

	opt = cfg_getopt(cfg, name);
	if (opt && opt->validcb2 && (*opt->validcb2)(cfg, opt, (void *)&value) != 0)
		return CFG_FAIL;

	return cfg_opt_setnint(opt, value, index);
}