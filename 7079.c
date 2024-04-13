DLLIMPORT int cfg_setcomment(cfg_t *cfg, const char *name, char *comment)
{
	return cfg_opt_setcomment(cfg_getopt(cfg, name), comment);
}