static void cfg_free_opt_array(cfg_opt_t *opts)
{
	int i;

	for (i = 0; opts[i].name; ++i) {
		free((void *)opts[i].name);
		if (opts[i].comment)
			free(opts[i].comment);
		if (opts[i].def.parsed)
			free(opts[i].def.parsed);
		if (opts[i].def.string)
			free((void *)opts[i].def.string);
		if (opts[i].subopts)
			cfg_free_opt_array(opts[i].subopts);
	}
	free(opts);
}