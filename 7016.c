DLLIMPORT int cfg_print(cfg_t *cfg, FILE *fp)
{
	return cfg_print_pff_indent(cfg, fp, NULL, 0);
}