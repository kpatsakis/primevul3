DLLIMPORT int cfg_setbool(cfg_t *cfg, const char *name, cfg_bool_t value)
{
	return cfg_setnbool(cfg, name, value, 0);
}