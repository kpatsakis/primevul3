static void cfg_init_defaults(cfg_t *cfg)
{
	int i;

	for (i = 0; cfg->opts && cfg->opts[i].name; i++) {
		int j;

		for (j = 0; j < i; ++j) {
			if (is_set(CFGF_NOCASE, cfg->opts[i].flags | cfg->opts[j].flags)) {
				if (strcasecmp(cfg->opts[i].name, cfg->opts[j].name))
					continue;
			} else {
				if (strcmp(cfg->opts[i].name, cfg->opts[j].name))
					continue;
			}
			/*
			 * There are two definitions of the same option name.
			 * What to do? It's a programming error and not an end
			 * user input error. Lets print a message and abort...
			 */
			cfg_error(cfg, _("duplicate option '%s' not allowed"),
				cfg->opts[i].name);
			break;
		}

		/* libConfuse doesn't handle default values for "simple" options */
		if (cfg->opts[i].simple_value.ptr || is_set(CFGF_NODEFAULT, cfg->opts[i].flags))
			continue;

		if (cfg->opts[i].type != CFGT_SEC) {
			cfg->opts[i].flags |= CFGF_DEFINIT;

			if (is_set(CFGF_LIST, cfg->opts[i].flags) || cfg->opts[i].def.parsed) {
				int xstate, ret = 0;
				char *buf;
				FILE *fp;

				/* If it's a list, but no default value was given,
				 * keep the option uninitialized.
				 */
				buf = cfg->opts[i].def.parsed;
				if (!buf || !buf[0])
					continue;

				/* setup scanning from the string specified for the
				 * "default" value, force the correct state and option
				 */

				if (is_set(CFGF_LIST, cfg->opts[i].flags))
					/* lists must be surrounded by {braces} */
					xstate = 3;
				else if (cfg->opts[i].type == CFGT_FUNC)
					xstate = 0;
				else
					xstate = 2;

				fp = fmemopen(buf, strlen(buf), "r");
				if (!fp) {
					/*
					 * fmemopen() on older GLIBC versions do not accept zero
					 * length buffers for some reason.  This is a workaround.
					 */
					if (strlen(buf) > 0)
						ret = STATE_ERROR;
				} else {
					cfg_scan_fp_begin(fp);

					do {
						ret = cfg_parse_internal(cfg, 1, xstate, &cfg->opts[i]);
						xstate = -1;
					} while (ret == STATE_CONTINUE);

					cfg_scan_fp_end();
					fclose(fp);
				}

				if (ret == STATE_ERROR) {
					/*
					 * If there was an error parsing the default string,
					 * the initialization of the default value could be
					 * inconsistent or empty. What to do? It's a
					 * programming error and not an end user input
					 * error. Lets print a message and abort...
					 */
					fprintf(stderr, "Parse error in default value '%s'"
						" for option '%s'\n", cfg->opts[i].def.parsed, cfg->opts[i].name);
					fprintf(stderr, "Check your initialization macros and the" " libConfuse documentation\n");
					abort();
				}
			} else {
				switch (cfg->opts[i].type) {
				case CFGT_INT:
					cfg_opt_setnint(&cfg->opts[i], cfg->opts[i].def.number, 0);
					break;

				case CFGT_FLOAT:
					cfg_opt_setnfloat(&cfg->opts[i], cfg->opts[i].def.fpnumber, 0);
					break;

				case CFGT_BOOL:
					cfg_opt_setnbool(&cfg->opts[i], cfg->opts[i].def.boolean, 0);
					break;

				case CFGT_STR:
					cfg_opt_setnstr(&cfg->opts[i], cfg->opts[i].def.string, 0);
					break;

				case CFGT_FUNC:
				case CFGT_PTR:
					break;

				default:
					cfg_error(cfg, "internal error in cfg_init_defaults(%s)", cfg->opts[i].name);
					break;
				}
			}

			/* The default value should only be returned if no value
			 * is given in the configuration file, so we set the RESET
			 * flag here. When/If cfg_setopt() is called, the value(s)
			 * will be freed and the flag unset.
			 */
			cfg->opts[i].flags |= CFGF_RESET;
			cfg->opts[i].flags &= ~CFGF_MODIFIED;
		} else if (!is_set(CFGF_MULTI, cfg->opts[i].flags)) {
			cfg_setopt(cfg, &cfg->opts[i], NULL);
			cfg->opts[i].flags |= CFGF_DEFINIT;
		}
	}
}