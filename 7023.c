DLLIMPORT int cfg_opt_rmtsec(cfg_opt_t *opt, const char *title)
{
	unsigned int i, n;

	if (!opt || !title) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	if (!is_set(CFGF_TITLE, opt->flags))
		return CFG_FAIL;

	n = cfg_opt_size(opt);
	for (i = 0; i < n; i++) {
		cfg_t *sec = cfg_opt_getnsec(opt, i);

		if (!sec || !sec->title)
			return CFG_FAIL;

		if (is_set(CFGF_NOCASE, opt->flags)) {
			if (strcasecmp(title, sec->title) == 0)
				break;
		} else {
			if (strcmp(title, sec->title) == 0)
				break;
		}
	}
	if (i == n)
		return CFG_FAIL;

	return cfg_opt_rmnsec(opt, i);
}