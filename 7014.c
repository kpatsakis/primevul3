DLLIMPORT int cfg_parse_fp(cfg_t *cfg, FILE *fp)
{
	int ret;

	if (!cfg || !fp) {
		errno = EINVAL;
		return CFG_PARSE_ERROR;
	}

	if (!cfg->filename)
		cfg->filename = strdup("FILE");
	if (!cfg->filename)
		return CFG_PARSE_ERROR;

	cfg->line = 1;
	cfg_scan_fp_begin(fp);
	ret = cfg_parse_internal(cfg, 0, -1, NULL);
	cfg_scan_fp_end();
	if (ret == STATE_ERROR)
		return CFG_PARSE_ERROR;

	return CFG_SUCCESS;
}