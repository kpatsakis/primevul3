DLLIMPORT int cfg_setfloat(cfg_t *cfg, const char *name, double value)
{
	return cfg_setnfloat(cfg, name, value, 0);
}