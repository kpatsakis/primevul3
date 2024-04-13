DLLIMPORT cfg_opt_t *cfg_getnopt(cfg_t *cfg, unsigned int index)
{
	unsigned int i;

	if (!cfg)
		return NULL;

	for (i = 0; cfg->opts && cfg->opts[i].name; i++) {
		if (i == index)
			return &cfg->opts[i];
	}

	return NULL;
}