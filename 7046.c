DLLIMPORT int cfg_addlist(cfg_t *cfg, const char *name, unsigned int nvalues, ...)
{
	va_list ap;
	cfg_opt_t *opt = cfg_getopt(cfg, name);

	if (!opt || !is_set(CFGF_LIST, opt->flags)) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	va_start(ap, nvalues);
	cfg_addlist_internal(opt, nvalues, ap);
	va_end(ap);

	return CFG_SUCCESS;
}