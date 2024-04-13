static long int cfg_opt_gettsecidx(cfg_opt_t *opt, const char *title)
{
	unsigned int i, n;

	n = cfg_opt_size(opt);
	for (i = 0; i < n; i++) {
		cfg_t *sec = cfg_opt_getnsec(opt, i);

		if (!sec || !sec->title)
			return -1;

		if (is_set(CFGF_NOCASE, opt->flags)) {
			if (strcasecmp(title, sec->title) == 0)
				return i;
		} else {
			if (strcmp(title, sec->title) == 0)
				return i;
		}
	}

	return -1;
}