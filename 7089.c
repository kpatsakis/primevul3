DLLIMPORT int cfg_rmsec(cfg_t *cfg, const char *name)
{
	cfg_opt_t *opt;
	long int index;

	opt = cfg_getopt_secidx(cfg, name, &index);
	return cfg_opt_rmnsec(opt, index);
}