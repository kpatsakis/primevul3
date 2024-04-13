DLLIMPORT char *cfg_getcomment(cfg_t *cfg, const char *name)
{
	return cfg_opt_getcomment(cfg_getopt(cfg, name));
}