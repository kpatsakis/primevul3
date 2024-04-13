static int cfg_print_pff_indent(cfg_t *cfg, FILE *fp,
				cfg_print_filter_func_t fb_pff, int indent)
{
	int i, result = CFG_SUCCESS;

	for (i = 0; cfg->opts[i].name; i++) {
		cfg_print_filter_func_t pff = cfg->pff ? cfg->pff : fb_pff;
		if (pff && pff(cfg, &cfg->opts[i]))
			continue;
		result += cfg_opt_print_pff_indent(&cfg->opts[i], fp, pff, indent);
	}

	return result;
}