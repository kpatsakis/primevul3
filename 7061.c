DLLIMPORT cfg_opt_t *cfg_getopt(cfg_t *cfg, const char *name)
{
	return cfg_getopt_secidx(cfg, name, NULL);
}