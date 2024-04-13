static cfg_opt_t *cfg_dupopt_array(cfg_opt_t *opts)
{
	int i;
	cfg_opt_t *dupopts;
	int n = cfg_numopts(opts);

	dupopts = calloc(n + 1, sizeof(cfg_opt_t));
	if (!dupopts)
		return NULL;

	memcpy(dupopts, opts, n * sizeof(cfg_opt_t));

	for (i = 0; i < n; i++) {
		/* Clear dynamic ptrs, protecting the original on failure */
		dupopts[i].name = NULL;
		dupopts[i].subopts = NULL;
		dupopts[i].def.parsed = NULL;
		dupopts[i].def.string = NULL;
		dupopts[i].comment = NULL;
	}

	for (i = 0; i < n; i++) {
		dupopts[i].name = strdup(opts[i].name);
		if (!dupopts[i].name)
			goto err;

		if (opts[i].subopts) {
			dupopts[i].subopts = cfg_dupopt_array(opts[i].subopts);
			if (!dupopts[i].subopts)
				goto err;
		}

		if (opts[i].def.parsed) {
			dupopts[i].def.parsed = strdup(opts[i].def.parsed);
			if (!dupopts[i].def.parsed)
				goto err;
		}

		if (opts[i].def.string) {
			dupopts[i].def.string = strdup(opts[i].def.string);
			if (!dupopts[i].def.string)
				goto err;
		}

		if (opts[i].comment) {
			dupopts[i].comment = strdup(opts[i].comment);
			if (!dupopts[i].comment)
				goto err;
		}
	}

	return dupopts;
err:
	cfg_free_opt_array(dupopts);
	return NULL;
}