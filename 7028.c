DLLIMPORT void *cfg_getptr(cfg_t *cfg, const char *name)
{
	return cfg_getnptr(cfg, name, 0);
}