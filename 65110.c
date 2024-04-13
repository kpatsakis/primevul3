lex_include_string(struct scanner *s, struct xkb_compose_table *table,
                   union lvalue *val_out)
{
    while (is_space(peek(s)))
        if (next(s) == '\n')
            return TOK_END_OF_LINE;

    s->token_line = s->line;
    s->token_column = s->column;
    s->buf_pos = 0;

    if (!chr(s, '\"')) {
        scanner_err(s, "include statement must be followed by a path");
        return TOK_ERROR;
    }

    while (!eof(s) && !eol(s) && peek(s) != '\"') {
        if (chr(s, '%')) {
            if (chr(s, '%')) {
                buf_append(s, '%');
            }
            else if (chr(s, 'H')) {
                const char *home = secure_getenv("HOME");
                if (!home) {
                    scanner_err(s, "%%H was used in an include statement, but the HOME environment variable is not set");
                    return TOK_ERROR;
                }
                if (!buf_appends(s, home)) {
                    scanner_err(s, "include path after expanding %%H is too long");
                    return TOK_ERROR;
                }
            }
            else if (chr(s, 'L')) {
                char *path = get_locale_compose_file_path(table->locale);
                if (!path) {
                    scanner_err(s, "failed to expand %%L to the locale Compose file");
                    return TOK_ERROR;
                }
                if (!buf_appends(s, path)) {
                    free(path);
                    scanner_err(s, "include path after expanding %%L is too long");
                    return TOK_ERROR;
                }
                free(path);
            }
            else if (chr(s, 'S')) {
                const char *xlocaledir = get_xlocaledir_path();
                if (!buf_appends(s, xlocaledir)) {
                    scanner_err(s, "include path after expanding %%S is too long");
                    return TOK_ERROR;
                }
            }
            else {
                scanner_err(s, "unknown %% format (%c) in include statement", peek(s));
                return TOK_ERROR;
            }
        } else {
            buf_append(s, next(s));
        }
    }
    if (!chr(s, '\"')) {
        scanner_err(s, "unterminated include statement");
        return TOK_ERROR;
    }
    if (!buf_append(s, '\0')) {
        scanner_err(s, "include path is too long");
        return TOK_ERROR;
    }
    val_out->string.str = s->buf;
    val_out->string.len = s->buf_pos;
    return TOK_INCLUDE_STRING;
}
