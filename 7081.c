DLLIMPORT int cfg_parse_buf(cfg_t *cfg, const char *buf)
{
	int ret;
	char *fn;
	FILE *fp;

	if (!cfg) {
		errno = EINVAL;
		return CFG_PARSE_ERROR;
	}

	if (!buf)
		return CFG_SUCCESS;

	fn = strdup("[buf]");
	if (!fn)
		return CFG_PARSE_ERROR;

	free(cfg->filename);
	cfg->filename = fn;

	fp = fmemopen((void *)buf, strlen(buf), "r");
	if (!fp) {
		/*
		 * fmemopen() on older GLIBC versions do not accept zero
		 * length buffers for some reason.  This is a workaround.
		 */
		if (strlen(buf) > 0)
			return CFG_FILE_ERROR;

		return CFG_SUCCESS;
	}

	ret = cfg_parse_fp(cfg, fp);
	fclose(fp);

	return ret;
}