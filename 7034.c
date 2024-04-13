static cfg_opt_t *cfg_getopt_array(cfg_opt_t *rootopts, int cfg_flags, const char *name)
{
	unsigned int i;
	cfg_opt_t *opts = rootopts;

	if (!rootopts || !name) {
		errno = EINVAL;
		return NULL;
	}

	while (name && *name) {
		cfg_t *seccfg;
		char *secname;
		size_t len = strcspn(name, "|");

		if (name[len] == 0 /*len == strlen(name) */ )
			/* no more subsections */
			break;

		if (len) {
			cfg_opt_t *secopt;

			secname = strndup(name, len);
			if (!secname)
				return NULL;

			secopt = cfg_getopt_array(opts, cfg_flags, secname);
			free(secname);
			if (!secopt) {
				/*fprintf(stderr, "section not found\n"); */
				return NULL;
			}
			if (secopt->type != CFGT_SEC) {
				/*fprintf(stderr, "not a section!\n"); */
				return NULL;
			}

			if (!is_set(CFGF_MULTI, secopt->flags) && (seccfg = cfg_opt_getnsec(secopt, 0)) != NULL)
				opts = seccfg->opts;
			else
				opts = secopt->subopts;

			if (!opts) {
				/*fprintf(stderr, "section have no subopts!?\n"); */
				return NULL;
			}
		}
		name += len;
		name += strspn(name, "|");
	}

	for (i = 0; opts[i].name; i++) {
		if (is_set(CFGF_NOCASE, cfg_flags)) {
			if (strcasecmp(opts[i].name, name) == 0)
				return &opts[i];
		} else {
			if (strcmp(opts[i].name, name) == 0)
				return &opts[i];
		}
	}

	return NULL;
}