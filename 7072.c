DLLIMPORT int cfg_rmnsec(cfg_t *cfg, const char *name, unsigned int index)
{
	return cfg_opt_rmnsec(cfg_getopt(cfg, name), index);
}