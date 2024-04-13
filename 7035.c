DLLIMPORT void *cfg_getnptr(cfg_t *cfg, const char *name, unsigned int index)
{
	return cfg_opt_getnptr(cfg_getopt(cfg, name), index);
}