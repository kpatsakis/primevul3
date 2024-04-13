static int cfg_opt_print_pff_indent(cfg_opt_t *opt, FILE *fp,
				    cfg_print_filter_func_t pff, int indent)
{
	if (!opt || !fp) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	if (is_set(CFGF_COMMENTS, opt->flags) && opt->comment) {
		cfg_indent(fp, indent);
		fprintf(fp, "/* %s */\n", opt->comment);
	}

	if (opt->type == CFGT_SEC) {
		cfg_t *sec;
		unsigned int i;

		for (i = 0; i < cfg_opt_size(opt); i++) {
			sec = cfg_opt_getnsec(opt, i);
			cfg_indent(fp, indent);
			if (is_set(CFGF_TITLE, opt->flags))
				fprintf(fp, "%s \"%s\" {\n", opt->name, cfg_title(sec));
			else
				fprintf(fp, "%s {\n", opt->name);
			cfg_print_pff_indent(sec, fp, pff, indent + 1);
			cfg_indent(fp, indent);
			fprintf(fp, "}\n");
		}
	} else if (opt->type != CFGT_FUNC && opt->type != CFGT_NONE) {
		if (is_set(CFGF_LIST, opt->flags)) {
			cfg_indent(fp, indent);
			fprintf(fp, "%s = {", opt->name);

			if (opt->nvalues) {
				unsigned int i;

				if (opt->pf)
					opt->pf(opt, 0, fp);
				else
					cfg_opt_nprint_var(opt, 0, fp);
				for (i = 1; i < opt->nvalues; i++) {
					fprintf(fp, ", ");
					if (opt->pf)
						opt->pf(opt, i, fp);
					else
						cfg_opt_nprint_var(opt, i, fp);
				}
			}

			fprintf(fp, "}");
		} else {
			cfg_indent(fp, indent);
			/* comment out the option if is not set */
			if (cfg_opt_size(opt) == 0 ||
			    (opt->type == CFGT_STR && !cfg_opt_getnstr(opt, 0)))
				fprintf(fp, "# ");
			fprintf(fp, "%s=", opt->name);
			if (opt->pf)
				opt->pf(opt, 0, fp);
			else
				cfg_opt_nprint_var(opt, 0, fp);
		}

		fprintf(fp, "\n");
	} else if (opt->pf) {
		cfg_indent(fp, indent);
		opt->pf(opt, 0, fp);
		fprintf(fp, "\n");
	}

	return CFG_SUCCESS;
}