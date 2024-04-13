DLLIMPORT signed long cfg_getnint(cfg_t *cfg, const char *name, unsigned int index)
{
	return cfg_opt_getnint(cfg_getopt(cfg, name), index);
}