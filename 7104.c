DLLIMPORT int cfg_opt_nprint_var(cfg_opt_t *opt, unsigned int index, FILE *fp)
{
	const char *str;

	if (!opt || !fp) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	switch (opt->type) {
	case CFGT_INT:
		fprintf(fp, "%ld", cfg_opt_getnint(opt, index));
		break;

	case CFGT_FLOAT:
		fprintf(fp, "%f", cfg_opt_getnfloat(opt, index));
		break;

	case CFGT_STR:
		str = cfg_opt_getnstr(opt, index);
		fprintf(fp, "\"");
		while (str && *str) {
			if (*str == '"')
				fprintf(fp, "\\\"");
			else if (*str == '\\')
				fprintf(fp, "\\\\");
			else
				fprintf(fp, "%c", *str);
			str++;
		}
		fprintf(fp, "\"");
		break;

	case CFGT_BOOL:
		fprintf(fp, "%s", cfg_opt_getnbool(opt, index) ? "true" : "false");
		break;

	case CFGT_NONE:
	case CFGT_SEC:
	case CFGT_FUNC:
	case CFGT_PTR:
	case CFGT_COMMENT:
		break;
	}

	return CFG_SUCCESS;
}