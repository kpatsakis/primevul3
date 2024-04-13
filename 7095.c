DLLIMPORT const char *cfg_title(cfg_t *cfg)
{
	if (cfg)
		return cfg->title;
	return NULL;
}