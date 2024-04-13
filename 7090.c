DLLIMPORT const char *cfg_opt_getstr(cfg_opt_t *opt)
{
	return cfg_opt_getnstr(opt, 0);
}