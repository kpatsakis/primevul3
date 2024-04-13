DLLIMPORT int cfg_opt_print(cfg_opt_t *opt, FILE *fp)
{
	return cfg_opt_print_pff_indent(opt, fp, NULL, 0);
}