static int ovl_parse_opt(char *opt, struct ovl_config *config)
{
	char *p;

	while ((p = strsep(&opt, ",")) != NULL) {
		int token;
		substring_t args[MAX_OPT_ARGS];

		if (!*p)
			continue;

		token = match_token(p, ovl_tokens, args);
		switch (token) {
		case OPT_UPPERDIR:
			kfree(config->upperdir);
			config->upperdir = match_strdup(&args[0]);
			if (!config->upperdir)
				return -ENOMEM;
			break;

		case OPT_LOWERDIR:
			kfree(config->lowerdir);
			config->lowerdir = match_strdup(&args[0]);
			if (!config->lowerdir)
				return -ENOMEM;
			break;

		case OPT_WORKDIR:
			kfree(config->workdir);
			config->workdir = match_strdup(&args[0]);
			if (!config->workdir)
				return -ENOMEM;
			break;

		default:
			return -EINVAL;
		}
	}
	return 0;
}
