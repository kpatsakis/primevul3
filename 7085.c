DLLIMPORT double cfg_getfloat(cfg_t *cfg, const char *name)
{
	return cfg_getnfloat(cfg, name, 0);
}