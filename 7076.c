DLLIMPORT cfg_print_func_t cfg_set_print_func(cfg_t *cfg, const char *name, cfg_print_func_t pf)
{
	return cfg_opt_set_print_func(cfg_getopt(cfg, name), pf);
}