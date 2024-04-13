DLLIMPORT int cfg_setint(cfg_t *cfg, const char *name, long int value)
{
	return cfg_setnint(cfg, name, value, 0);
}