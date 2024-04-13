DLLIMPORT char *cfg_getstr(cfg_t *cfg, const char *name)
{
	return cfg_getnstr(cfg, name, 0);
}