DLLIMPORT int cfg_setmulti(cfg_t *cfg, const char *name, unsigned int nvalues, char **values)
{
	cfg_opt_t *opt;

	if (!cfg || !name || !values) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	opt = cfg_getopt(cfg, name);
	if (!opt) {
		errno = ENOENT;
		return CFG_FAIL;
	}

	return cfg_opt_setmulti(cfg, opt, nvalues, values);
}