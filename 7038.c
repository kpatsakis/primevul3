DLLIMPORT int cfg_rmtsec(cfg_t *cfg, const char *name, const char *title)
{
	return cfg_opt_rmtsec(cfg_getopt(cfg, name), title);
}