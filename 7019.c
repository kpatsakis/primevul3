DLLIMPORT int cfg_include(cfg_t *cfg, cfg_opt_t *opt, int argc, const char **argv)
{
	(void)opt;		/* Unused in this predefined include FUNC */

	if (!cfg || !argv) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	if (argc != 1) {
		cfg_error(cfg, _("wrong number of arguments to cfg_include()"));
		return 1;
	}

	return cfg_lexer_include(cfg, argv[0]);
}