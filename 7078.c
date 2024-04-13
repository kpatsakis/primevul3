DLLIMPORT int cfg_setstr(cfg_t *cfg, const char *name, const char *value)
{
	return cfg_setnstr(cfg, name, value, 0);
}