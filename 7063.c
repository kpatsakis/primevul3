DLLIMPORT cfg_t *cfg_opt_getnsec(cfg_opt_t *opt, unsigned int index)
{
	if (!opt || opt->type != CFGT_SEC) {
		errno = EINVAL;
		return NULL;
	}

	if (opt->values && index < opt->nvalues)
		return opt->values[index]->section;

	errno = ENOENT;
	return NULL;
}