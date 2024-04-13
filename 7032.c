DLLIMPORT unsigned int cfg_num(cfg_t *cfg)
{
	if (!cfg)
		return 0;

	return (unsigned int)cfg_numopts(cfg->opts);
}