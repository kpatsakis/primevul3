static cfg_opt_t *cfg_getopt_leaf(cfg_t *cfg, const char *name)
{
	unsigned int i;

	for (i = 0; cfg->opts && cfg->opts[i].name; i++) {
		if (is_set(CFGF_NOCASE, cfg->flags)) {
			if (strcasecmp(cfg->opts[i].name, name) == 0)
				return &cfg->opts[i];
		} else {
			if (strcmp(cfg->opts[i].name, name) == 0)
				return &cfg->opts[i];
		}
	}

	return NULL;
}