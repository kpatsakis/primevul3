DLLIMPORT cfg_t *cfg_init(cfg_opt_t *opts, cfg_flag_t flags)
{
	cfg_t *cfg;

	cfg = calloc(1, sizeof(cfg_t));
	if (!cfg)
		return NULL;

	cfg->name = strdup("root");
	if (!cfg->name) {
		free(cfg);
		return NULL;
	}

	cfg->opts = cfg_dupopt_array(opts);
	if (!cfg->opts) {
		free(cfg->name);
		free(cfg);
		return NULL;
	}

	cfg->flags = flags;
	cfg->filename = NULL;
	cfg->line = 0;
	cfg->errfunc = NULL;

#if defined(ENABLE_NLS) && defined(HAVE_GETTEXT)
	bindtextdomain(PACKAGE, LOCALEDIR);
#endif

	cfg_init_defaults(cfg);

	return cfg;
}