DLLIMPORT cfg_bool_t cfg_getnbool(cfg_t *cfg, const char *name, unsigned int index)
{
	return cfg_opt_getnbool(cfg_getopt(cfg, name), index);
}