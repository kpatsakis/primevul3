DLLIMPORT cfg_bool_t cfg_getbool(cfg_t *cfg, const char *name)
{
	return cfg_getnbool(cfg, name, 0);
}