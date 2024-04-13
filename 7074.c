DLLIMPORT double cfg_getnfloat(cfg_t *cfg, const char *name, unsigned int index)
{
	return cfg_opt_getnfloat(cfg_getopt(cfg, name), index);
}