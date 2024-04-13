DLLIMPORT int cfg_free_value(cfg_opt_t *opt)
{
	if (!opt) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	if (opt->comment && !is_set(CFGF_RESET, opt->flags)) {
		free(opt->comment);
		opt->comment = NULL;
	}

	if (opt->values) {
		unsigned int i;

		for (i = 0; i < opt->nvalues; i++) {
			if (opt->type == CFGT_STR) {
				free((void *)opt->values[i]->string);
			} else if (opt->type == CFGT_SEC) {
				opt->values[i]->section->path = NULL; /* Global search path */
				cfg_free(opt->values[i]->section);
			} else if (opt->type == CFGT_PTR && opt->freecb && opt->values[i]->ptr) {
				(opt->freecb) (opt->values[i]->ptr);
			}
			free(opt->values[i]);
		}
		free(opt->values);
	}

	opt->values  = NULL;
	opt->nvalues = 0;

	return CFG_SUCCESS;
}