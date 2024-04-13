DLLIMPORT signed long cfg_getint(cfg_t *cfg, const char *name)
{
	return cfg_getnint(cfg, name, 0);
}