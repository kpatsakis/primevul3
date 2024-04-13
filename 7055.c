DLLIMPORT unsigned int cfg_size(cfg_t *cfg, const char *name)
{
	return cfg_opt_size(cfg_getopt(cfg, name));
}