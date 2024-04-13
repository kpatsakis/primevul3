DLLIMPORT cfg_value_t *cfg_setopt(cfg_t *cfg, cfg_opt_t *opt, const char *value)
{
	cfg_value_t *val = NULL;
	const char *s;
	char *endptr;
	long int i;
	double f;
	void *p;
	int b;

	if (!cfg || !opt) {
		errno = EINVAL;
		return NULL;
	}

	if (opt->simple_value.ptr) {
		if (opt->type == CFGT_SEC) {
			errno = EINVAL;
			return NULL;
		}
		val = (cfg_value_t *)opt->simple_value.ptr;
	} else {
		if (is_set(CFGF_RESET, opt->flags)) {
			cfg_free_value(opt);
			opt->flags &= ~CFGF_RESET;
		}

		if (opt->nvalues == 0 || is_set(CFGF_MULTI, opt->flags) || is_set(CFGF_LIST, opt->flags)) {
			val = NULL;

			if (opt->type == CFGT_SEC && is_set(CFGF_TITLE, opt->flags)) {
				unsigned int i;

				/* XXX: Check if there already is a section with the same title. */

				/*
				 * Check there are either no sections at
				 * all, or a non-NULL section title.
				 */
				if (opt->nvalues != 0 && !value) {
					errno = EINVAL;
					return NULL;
				}

				for (i = 0; i < opt->nvalues && val == NULL; i++) {
					cfg_t *sec = opt->values[i]->section;

					if (is_set(CFGF_NOCASE, cfg->flags)) {
						if (strcasecmp(value, sec->title) == 0)
							val = opt->values[i];
					} else {
						if (strcmp(value, sec->title) == 0)
							val = opt->values[i];
					}
				}

				if (val && is_set(CFGF_NO_TITLE_DUPES, opt->flags)) {
					cfg_error(cfg, _("found duplicate title '%s'"), value);
					return NULL;
				}
			}

			if (!val) {
				val = cfg_addval(opt);
				if (!val)
					return NULL;
			}
		} else {
			val = opt->values[0];
		}
	}

	switch (opt->type) {
	case CFGT_INT:
		if (opt->parsecb) {
			if ((*opt->parsecb) (cfg, opt, value, &i) != 0)
				return NULL;
		} else {
			const char *str;
			int radix = 0;

			if (!value) {
				errno = EINVAL;
				return NULL;
			}

			str = value;
			if (value[0] == '0') {
				switch (value[1]) {
				case 'b':
					radix = 2;
					str = &value[2];
					break;
				case 'x':
					radix = 16;
					str = &value[2];
					break;
				default:
					radix = 8;
					str = &value[1];
				}
			}

			i = strtol(str, &endptr, radix);
			if (*endptr != '\0') {
				cfg_error(cfg, _("invalid integer value for option '%s'"), opt->name);
				return NULL;
			}

			if (errno == ERANGE) {
				cfg_error(cfg, _("integer value for option '%s' is out of range"), opt->name);
				return NULL;
			}
		}
		val->number = i;
		break;

	case CFGT_FLOAT:
		if (opt->parsecb) {
			if ((*opt->parsecb) (cfg, opt, value, &f) != 0)
				return NULL;
		} else {
			if (!value) {
				errno = EINVAL;
				return NULL;
			}
			f = strtod(value, &endptr);
			if (*endptr != '\0') {
				cfg_error(cfg, _("invalid floating point value for option '%s'"), opt->name);
				return NULL;
			}
			if (errno == ERANGE) {
				cfg_error(cfg, _("floating point value for option '%s' is out of range"), opt->name);
				return NULL;
			}
		}
		val->fpnumber = f;
		break;

	case CFGT_STR:
		if (opt->parsecb) {
			s = NULL;
			if ((*opt->parsecb) (cfg, opt, value, &s) != 0)
				return NULL;
		} else {
			s = value;
		}

		if (!s) {
			errno = EINVAL;
			return NULL;
		}

		free(val->string);
		val->string = strdup(s);
		if (!val->string)
			return NULL;
		break;

	case CFGT_SEC:
		if (is_set(CFGF_MULTI, opt->flags) || val->section == NULL) {
			if (val->section) {
				val->section->path = NULL; /* Global search path */
				cfg_free(val->section);
			}
			val->section = calloc(1, sizeof(cfg_t));
			if (!val->section)
				return NULL;

			val->section->name = strdup(opt->name);
			if (!val->section->name) {
				free(val->section);
				return NULL;
			}

			val->section->flags = cfg->flags;
			if (is_set(CFGF_KEYSTRVAL, opt->flags))
				val->section->flags |= CFGF_KEYSTRVAL;

			val->section->filename = cfg->filename ? strdup(cfg->filename) : NULL;
			if (cfg->filename && !val->section->filename) {
				free(val->section->name);
				free(val->section);
				return NULL;
			}

			val->section->line = cfg->line;
			val->section->errfunc = cfg->errfunc;
			val->section->title = value ? strdup(value) : NULL;
			if (value && !val->section->title) {
				free(val->section->filename);
				free(val->section->name);
				free(val->section);
				return NULL;
			}

			val->section->opts = cfg_dupopt_array(opt->subopts);
			if (!val->section->opts) {
				if (val->section->title)
					free(val->section->title);
				if (val->section->filename)
					free(val->section->filename);
				free(val->section->name);
				free(val->section);
				return NULL;
			}
		}
		if (!is_set(CFGF_DEFINIT, opt->flags))
			cfg_init_defaults(val->section);
		break;

	case CFGT_BOOL:
		if (opt->parsecb) {
			if ((*opt->parsecb) (cfg, opt, value, &b) != 0)
				return NULL;
		} else {
			b = cfg_parse_boolean(value);
			if (b == -1) {
				cfg_error(cfg, _("invalid boolean value for option '%s'"), opt->name);
				return NULL;
			}
		}
		val->boolean = (cfg_bool_t)b;
		break;

	case CFGT_PTR:
		if (!opt->parsecb) {
			errno = EINVAL;
			return NULL;
		}

		if ((*opt->parsecb) (cfg, opt, value, &p) != 0)
			return NULL;
		if (val->ptr && opt->freecb)
			opt->freecb(val->ptr);
		val->ptr = p;
		break;

	default:
		cfg_error(cfg, "internal error in cfg_setopt(%s, %s)", opt->name, (value) ? (value) : "NULL");
		return NULL;
	}

	opt->flags |= CFGF_MODIFIED;

	return val;
}