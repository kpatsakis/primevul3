DLLIMPORT cfg_t *cfg_addtsec(cfg_t *cfg, const char *name, const char *title)
{
	cfg_opt_t *opt;
	cfg_value_t *val;

	if (cfg_gettsec(cfg, name, title))
		return NULL;

	opt = cfg_getopt(cfg, name);
	if (!opt) {
		cfg_error(cfg, _("no such option '%s'"), name);
		return NULL;
	}
	val = cfg_setopt(cfg, opt, title);
	if (!val)
		return NULL;

	val->section->path = cfg->path; /* Remember global search path. */
	val->section->line = 1;
	val->section->errfunc = cfg->errfunc;

	return val->section;
}