DLLIMPORT cfg_t *cfg_gettsec(cfg_t *cfg, const char *name, const char *title)
{
	return cfg_opt_gettsec(cfg_getopt(cfg, name), title);
}