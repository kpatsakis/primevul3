DLLIMPORT cfg_t *cfg_getnsec(cfg_t *cfg, const char *name, unsigned int index)
{
	return cfg_opt_getnsec(cfg_getopt(cfg, name), index);
}