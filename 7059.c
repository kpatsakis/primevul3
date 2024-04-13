DLLIMPORT cfg_errfunc_t cfg_set_error_function(cfg_t *cfg, cfg_errfunc_t errfunc)
{
	cfg_errfunc_t old;

	if (!cfg) {
		errno = EINVAL;
		return NULL;
	}

	old = cfg->errfunc;
	cfg->errfunc = errfunc;

	return old;
}