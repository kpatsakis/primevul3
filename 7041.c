DLLIMPORT void *cfg_opt_getnptr(cfg_opt_t *opt, unsigned int index)
{
	if (!opt || opt->type != CFGT_PTR) {
		errno = EINVAL;
		return NULL;
	}

	if (opt->values && index < opt->nvalues)
		return opt->values[index]->ptr;
	if (opt->simple_value.ptr)
		return *opt->simple_value.ptr;

	return NULL;
}