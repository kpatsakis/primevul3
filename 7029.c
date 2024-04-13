DLLIMPORT int cfg_free(cfg_t *cfg)
{
	int i;
	int isroot = 0;

	if (!cfg) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	if (cfg->comment)
		free(cfg->comment);

	for (i = 0; cfg->opts[i].name; ++i)
		cfg_free_value(&cfg->opts[i]);

	cfg_free_opt_array(cfg->opts);
	cfg_free_searchpath(cfg->path);

	if (cfg->name) {
		isroot = !strcmp(cfg->name, "root");
		free(cfg->name);
	}
	if (cfg->title)
		free(cfg->title);
	if (cfg->filename)
		free(cfg->filename);

	free(cfg);
	if (isroot)
		cfg_yylex_destroy();

	return CFG_SUCCESS;
}