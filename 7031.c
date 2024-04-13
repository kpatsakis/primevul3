DLLIMPORT int cfg_numopts(cfg_opt_t *opts)
{
	int n;

	for (n = 0; opts && opts[n].name; n++)
		/* do nothing */ ;
	return n;
}