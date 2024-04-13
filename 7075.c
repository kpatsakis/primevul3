static int cfg_addlist_internal(cfg_opt_t *opt, unsigned int nvalues, va_list ap)
{
	int result = CFG_FAIL;
	unsigned int i;

	for (i = 0; i < nvalues; i++) {
		switch (opt->type) {
		case CFGT_INT:
			result = cfg_opt_setnint(opt, va_arg(ap, int), opt->nvalues);
			break;

		case CFGT_FLOAT:
			result = cfg_opt_setnfloat(opt, va_arg(ap, double), opt->nvalues);
			break;

		case CFGT_BOOL:
			result = cfg_opt_setnbool(opt, va_arg(ap, cfg_bool_t), opt->nvalues);
			break;

		case CFGT_STR:
			result = cfg_opt_setnstr(opt, va_arg(ap, char *), opt->nvalues);
			break;

		case CFGT_FUNC:
		case CFGT_SEC:
		default:
			result = CFG_SUCCESS;
			break;
		}
	}

	return result;
}