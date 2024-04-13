DLLIMPORT int cfg_opt_print_indent(cfg_opt_t *opt, FILE *fp, int indent)
{
	return cfg_opt_print_pff_indent(opt, fp, NULL, indent);
}