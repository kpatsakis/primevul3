static int cfg_parse_internal(cfg_t *cfg, int level, int force_state, cfg_opt_t *force_opt)
{
	int state = 0;
	char *comment = NULL;
	char *opttitle = NULL;
	cfg_opt_t *opt = NULL;
	cfg_value_t *val = NULL;
	cfg_opt_t funcopt = CFG_STR(NULL, NULL, 0);

	int ignore = 0;		/* ignore until this token, traverse parser w/o error */
	int num_values = 0;	/* number of values found for a list option */
	int rc;

	if (force_state != -1)
		state = force_state;
	if (force_opt)
		opt = force_opt;

	while (1) {
		int tok = cfg_yylex(cfg);

		if (tok == 0) {
			/* lexer.l should have called cfg_error() */
			goto error;
		}

		if (tok == EOF) {
			if (state != 0) {
				cfg_error(cfg, _("premature end of file"));
				goto error;
			}

			if (opt && is_set(CFGF_DEPRECATED, opt->flags))
				cfg_handle_deprecated(cfg, opt);

			if (comment)
				free(comment);

			return STATE_EOF;
		}

		switch (state) {
		case 0:	/* expecting an option name */
			if (opt && is_set(CFGF_DEPRECATED, opt->flags))
				cfg_handle_deprecated(cfg, opt);

			switch (tok) {
			case '}':
				if (level == 0) {
					cfg_error(cfg, _("unexpected closing brace"));
					goto error;
				}
				if (comment)
					free(comment);

				return STATE_EOF;

			case CFGT_STR:
				break;

			case CFGT_COMMENT:
				if (!is_set(CFGF_COMMENTS, cfg->flags))
					continue;

				if (comment)
					free(comment);
				comment = strdup(cfg_yylval);
				continue;

			default:
				cfg_error(cfg, _("unexpected token '%s'"), cfg_yylval);
				goto error;
			}

			opt = cfg_getopt(cfg, cfg_yylval);
			if (!opt) {
				if (is_set(CFGF_IGNORE_UNKNOWN, cfg->flags)) {
					state = 10;
					break;
				}

				/* Not found, is it a dynamic key-value section? */
				if (is_set(CFGF_KEYSTRVAL, cfg->flags)) {
					opt = cfg_addopt(cfg, cfg_yylval);
					if (!opt)
						goto error;

					state = 1;
					break;
				}

				goto error;
			}

			if (opt->type == CFGT_SEC) {
				if (is_set(CFGF_TITLE, opt->flags))
					state = 6;
				else
					state = 5;
			} else if (opt->type == CFGT_FUNC) {
				state = 7;
			} else {
				state = 1;
			}
			break;

		case 1:	/* expecting an equal sign or plus-equal sign */
			if (!opt)
				goto error;

			if (tok == '+') {
				if (!is_set(CFGF_LIST, opt->flags)) {
					cfg_error(cfg, _("attempt to append to non-list option '%s'"), opt->name);
					goto error;
				}
				/* Even if the reset flag was set by
				 * cfg_init_defaults, appending to the defaults
				 * should be ok.
				 */
				opt->flags &= ~CFGF_RESET;
			} else if (tok == '=') {
				/* set the (temporary) reset flag to clear the old
				 * values, since we obviously didn't want to append */
				opt->flags |= CFGF_RESET;
			} else {
				cfg_error(cfg, _("missing equal sign after option '%s'"), opt->name);
				goto error;
			}

			opt->flags |= CFGF_MODIFIED;

			if (is_set(CFGF_LIST, opt->flags)) {
				state = 3;
				num_values = 0;
			} else {
				state = 2;
			}
			break;

		case 2:	/* expecting an option value */
			if (tok == '}' && opt && is_set(CFGF_LIST, opt->flags)) {
				state = 0;
				if (num_values == 0 && is_set(CFGF_RESET, opt->flags))
					/* Reset flags was set, and the empty list was
					 * specified. Free all old values. */
					cfg_free_value(opt);
				break;
			}

			if (tok != CFGT_STR) {
				cfg_error(cfg, _("unexpected token '%s'"), cfg_yylval);
				goto error;
			}

			if (cfg_setopt(cfg, opt, cfg_yylval) == NULL)
				goto error;

			if (opt && opt->validcb && (*opt->validcb) (cfg, opt) != 0)
				goto error;

			/* Inherit last read comment */
			cfg_opt_setcomment(opt, comment);
			if (comment)
				free(comment);
			comment = NULL;

			if (opt && is_set(CFGF_LIST, opt->flags)) {
				++num_values;
				state = 4;
			} else {
				state = 0;
			}
			break;

		case 3:	/* expecting an opening brace for a list option */
			if (tok != '{') {
				if (tok != CFGT_STR) {
					cfg_error(cfg, _("unexpected token '%s'"), cfg_yylval);
					goto error;
				}

				if (cfg_setopt(cfg, opt, cfg_yylval) == NULL)
					goto error;
				if (opt && opt->validcb && (*opt->validcb) (cfg, opt) != 0)
					goto error;
				++num_values;
				state = 0;
			} else {
				state = 2;
			}
			break;

		case 4:	/* expecting a separator for a list option, or closing (list) brace */
			if (tok == ',') {
				state = 2;
			} else if (tok == '}') {
				state = 0;
				if (opt && opt->validcb && (*opt->validcb) (cfg, opt) != 0)
					goto error;
			} else {
				cfg_error(cfg, _("unexpected token '%s'"), cfg_yylval);
				goto error;
			}
			break;

		case 5:	/* expecting an opening brace for a section */
			if (tok != '{') {
				cfg_error(cfg, _("missing opening brace for section '%s'"), opt ? opt->name : "");
				goto error;
			}

			val = cfg_setopt(cfg, opt, opttitle);
			if (!val)
				goto error;

			if (opttitle)
				free(opttitle);
			opttitle = NULL;

			val->section->path = cfg->path; /* Remember global search path */
			val->section->line = cfg->line;
			val->section->errfunc = cfg->errfunc;
			rc = cfg_parse_internal(val->section, level + 1, -1, NULL);
			if (rc != STATE_EOF)
				goto error;

			cfg->line = val->section->line;
			if (opt && opt->validcb && (*opt->validcb) (cfg, opt) != 0)
				goto error;
			state = 0;
			break;

		case 6:	/* expecting a title for a section */
			if (tok != CFGT_STR) {
				cfg_error(cfg, _("missing title for section '%s'"), opt ? opt->name : "");
				goto error;
			} else {
				opttitle = strdup(cfg_yylval);
				if (!opttitle)
					goto error;
			}
			state = 5;
			break;

		case 7:	/* expecting an opening parenthesis for a function */
			if (tok != '(') {
				cfg_error(cfg, _("missing parenthesis for function '%s'"), opt ? opt->name : "");
				goto error;
			}
			state = 8;
			break;

		case 8:	/* expecting a function parameter or a closing paren */
			if (tok == ')') {
				if (call_function(cfg, opt, &funcopt))
					goto error;
				state = 0;
			} else if (tok == CFGT_STR) {
				val = cfg_addval(&funcopt);
				if (!val)
					goto error;

				val->string = strdup(cfg_yylval);
				if (!val->string)
					goto error;

				state = 9;
			} else {
				cfg_error(cfg, _("syntax error in call of function '%s'"), opt ? opt->name : "");
				goto error;
			}
			break;

		case 9:	/* expecting a comma in a function or a closing paren */
			if (tok == ')') {
				if (call_function(cfg, opt, &funcopt))
					goto error;
				state = 0;
			} else if (tok == ',') {
				state = 8;
			} else {
				cfg_error(cfg, _("syntax error in call of function '%s'"), opt ? opt->name : "");
				goto error;
			}
			break;

		case 10: /* unknown option, mini-discard parser states: 10-15 */
			if (comment) {
				free(comment);
				comment = NULL;
			}

			if (tok == '+') {
				ignore = '=';
				state = 13; /* Append to list, should be followed by '=' */
			} else if (tok == '=') {
				ignore = 0;
				state = 14; /* Assignment, regular handling */
			} else if (tok == '(') {
				ignore = ')';
				state = 13; /* Function, ignore until end of param list */
			} else if (tok == '{') {
				state = 12; /* Section, ignore all until closing brace */
			} else if (tok == CFGT_STR) {
				state = 11; /* No '=' ... must be a titled section */
			} else if (tok == '}' && force_state == 10) {
				if (comment)
					free(comment);

				return STATE_CONTINUE;
			}
			break;

		case 11: /* unknown option, expecting start of title section */
			if (tok != '{') {
				cfg_error(cfg, _("unexpected token '%s'"), cfg_yylval);
				goto error;
			}
			state = 12;
			break;

		case 12: /* unknown option, recursively ignore entire sub-section */
			rc = cfg_parse_internal(cfg, level + 1, 10, NULL);
			if (rc != STATE_CONTINUE)
				goto error;
			ignore = '}';
			state = 13;
			break;

		case 13: /* unknown option, consume tokens silently until end of func/list */
			if (tok != ignore)
				break;

			if (ignore == '=') {
				ignore = 0;
				state = 14;
				break;
			}

			/* Are we done with recursive ignore of sub-section? */
			if (force_state == 10) {
				if (comment)
					free(comment);

				return STATE_CONTINUE;
			}

			ignore = 0;
			state = 0;
			break;

		case 14: /* unknown option, assuming value or start of list */
			if (tok == '{') {
				ignore = '}';
				state = 13;
				break;
			}

			if (tok != CFGT_STR) {
				cfg_error(cfg, _("unexpected token '%s'"), cfg_yylval);
				goto error;
			}

			ignore = 0;
			if (force_state == 10)
				state = 15;
			else
				state = 0;
			break;

		case 15: /* unknown option, dummy read of next parameter in sub-section */
			state = 10;
			break;

		default:
			cfg_error(cfg, _("Internal error in cfg_parse_internal(), unknown state %d"), state);
			goto error;
		}
	}

	if (comment)
		free(comment);

	return STATE_EOF;

error:
	if (opttitle)
		free(opttitle);
	if (comment)
		free(comment);

	return STATE_ERROR;
}