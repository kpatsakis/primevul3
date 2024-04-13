static cfg_opt_t *cfg_addopt(cfg_t *cfg, char *key)
{
	int num = cfg_num(cfg);
	cfg_opt_t *opts;

	opts = reallocarray(cfg->opts, num + 2, sizeof(cfg_opt_t));
	if (!opts)
		return NULL;

	/* Write new opt to previous CFG_END() marker */
	cfg->opts = opts;
	cfg->opts[num].name = strdup(key);
	cfg->opts[num].type = CFGT_STR;

	if (!cfg->opts[num].name) {
		free(opts);
		return NULL;
	}

	/* Set new CFG_END() */
	memset(&cfg->opts[num + 1], 0, sizeof(cfg_opt_t));

	return &cfg->opts[num];
}