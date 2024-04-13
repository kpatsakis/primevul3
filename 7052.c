DLLIMPORT char *cfg_getnstr(cfg_t *cfg, const char *name, unsigned int index)
{
	return cfg_opt_getnstr(cfg_getopt(cfg, name), index);
}