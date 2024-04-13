DLLIMPORT int cfg_print_indent(cfg_t *cfg, FILE *fp, int indent)
{
	return cfg_print_pff_indent(cfg, fp, NULL, indent);
}