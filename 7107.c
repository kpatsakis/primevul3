static cfg_opt_t *cfg_getopt_secidx(cfg_t *cfg, const char *name,
				    long int *index)
{
	cfg_opt_t *opt = NULL;
	cfg_t *sec = cfg;

	if (!cfg || !cfg->name || !name || !*name) {
		errno = EINVAL;
		return NULL;
	}

	while (name && *name) {
		char *title = NULL;
		long int i = -1;
		char *secname;
		size_t len;

		len = strcspn(name, "|=");
		if (!index && name[len] == 0 /*len == strlen(name) */ )
			/* no more subsections */
			break;

		if (!len)
			break;

		secname = strndup(name, len);
		if (!secname)
			return NULL;

		do {
			char *endptr;

			opt = cfg_getopt_leaf(sec, secname);
			if (!opt || opt->type != CFGT_SEC) {
				opt = NULL;
				break;
			}
			if (name[len] != '=') {
				/* non-multi, and backwards compat */
				i = 0;
				break;
			}
			if (!is_set(CFGF_MULTI, opt->flags))
				break;
			name += len + 1;
			title = parse_title(name, &len);
			if (!title)
				break;
			if (is_set(CFGF_TITLE, opt->flags)) {
				i = cfg_opt_gettsecidx(opt, title);
				break;
			}

			i = strtol(title, &endptr, 0);
			if (*endptr != '\0')
				i = -1;
		} while(0);

		if (index)
			*index = i;

		sec = i >= 0 ? cfg_opt_getnsec(opt, i) : NULL;
		if (!sec && !is_set(CFGF_IGNORE_UNKNOWN, cfg->flags)) {
			if (opt && !is_set(CFGF_MULTI, opt->flags))
				cfg_error(cfg, _("no such option '%s'"), secname);
			else if (title)
				cfg_error(cfg, _("no sub-section '%s' in '%s'"), title, secname);
			else
				cfg_error(cfg, _("no sub-section title/index for '%s'"), secname);
		}

		free(secname);
		if (title)
			free(title);
		if (!sec)
			return NULL;

		name += len;
		name += strspn(name, "|");
	}

	if (!index) {
		opt = cfg_getopt_leaf(sec, name);

		if (!opt && !is_set(CFGF_IGNORE_UNKNOWN, cfg->flags) && !is_set(CFGF_KEYSTRVAL, sec->flags))
			cfg_error(cfg, _("no such option '%s'"), name);
	}

	return opt;
}