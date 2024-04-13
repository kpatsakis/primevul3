static int call_function(cfg_t *cfg, cfg_opt_t *opt, cfg_opt_t *funcopt)
{
	int ret;
	const char **argv;
	unsigned int i;

	if (!cfg || !opt ||!funcopt) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	/*
	 * create am argv string vector and call the registered function
	 */
	argv = calloc(funcopt->nvalues, sizeof(char *));
	if (!argv)
		return CFG_FAIL;

	for (i = 0; i < funcopt->nvalues; i++)
		argv[i] = funcopt->values[i]->string;

	ret = (*opt->func) (cfg, opt, funcopt->nvalues, argv);
	cfg_free_value(funcopt);
	free(argv);

	return ret;
}