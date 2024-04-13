DLLIMPORT cfg_print_func_t cfg_opt_set_print_func(cfg_opt_t *opt, cfg_print_func_t pf)
{
	cfg_print_func_t oldpf;

	if (!opt) {
		errno = EINVAL;
		return NULL;
	}

	oldpf = opt->pf;
	opt->pf = pf;

	return oldpf;
}