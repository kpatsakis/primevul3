DLLIMPORT int cfg_parse(cfg_t *cfg, const char *filename)
{
	int ret;
	char *fn;
	FILE *fp;

	if (!cfg || !filename) {
		errno = EINVAL;
		return CFG_FILE_ERROR;
	}

	if (cfg->path)
		fn = cfg_searchpath(cfg->path, filename);
	else
		fn = cfg_tilde_expand(filename);
	if (!fn)
		return CFG_FILE_ERROR;

	free(cfg->filename);
	cfg->filename = fn;

	fp = fopen(cfg->filename, "r");
	if (!fp)
		return CFG_FILE_ERROR;

	ret = cfg_parse_fp(cfg, fp);
	fclose(fp);

	return ret;
}