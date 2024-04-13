DLLIMPORT cfg_t *cfg_opt_gettsec(cfg_opt_t *opt, const char *title)
{
	long int i;

	if (!opt || !title) {
		errno = EINVAL;
		return NULL;
	}

	if (!is_set(CFGF_TITLE, opt->flags)) {
		errno = EINVAL;
		return NULL;
	}

	i = cfg_opt_gettsecidx(opt, title);
	if (i >= 0)
		return cfg_opt_getnsec(opt, i);

	errno = ENOENT;
	return NULL;
}