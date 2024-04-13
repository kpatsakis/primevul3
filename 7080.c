DLLIMPORT void cfg_error(cfg_t *cfg, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);

	if (cfg && cfg->errfunc)
		(*cfg->errfunc) (cfg, fmt, ap);
	else {
		if (cfg && cfg->filename && cfg->line)
			fprintf(stderr, "%s:%d: ", cfg->filename, cfg->line);
		else if (cfg && cfg->filename)
			fprintf(stderr, "%s: ", cfg->filename);
		vfprintf(stderr, fmt, ap);
		fprintf(stderr, "\n");
	}

	va_end(ap);
}