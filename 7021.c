DLLIMPORT int cfg_setnbool(cfg_t *cfg, const char *name, cfg_bool_t value, unsigned int index)
{
	return cfg_opt_setnbool(cfg_getopt(cfg, name), value, index);
}