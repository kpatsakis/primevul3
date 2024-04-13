DLLIMPORT cfg_print_filter_func_t cfg_set_print_filter_func(cfg_t *cfg, cfg_print_filter_func_t pff)
{
	cfg_print_filter_func_t old;

	if (!cfg) {
		errno = EINVAL;
		return NULL;
	}

	old = cfg->pff;
	cfg->pff = pff;

	return old;
}