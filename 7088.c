DLLIMPORT const char *cfg_name(cfg_t *cfg)
{
	if (cfg)
		return cfg->name;
	return NULL;
}