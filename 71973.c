tokenize(apr_pool_t *pool, const char *str, bool ignore_whitespace,
             char **error)
{
    apr_array_header_t *tokens = apr_array_make(pool, 10, sizeof(Token));
    const char *p, *start;

    *error = NULL;
    p = start = str;
    while(*p) {
        if (apr_isspace(*p)) {  /* whitespace */
            p++;
            while(*p && apr_isspace(*p)) p++;
            if (!ignore_whitespace) {
                push_token(tokens, TOKEN_WHITESPACE, str, start, p);
            }
            start = p;
        }
        else if (apr_isalpha(*p)) { /* identifier: must begin with
                                       alpha then any alphanumeric or
                                       underscore */
            p++;
            while(*p && (apr_isalnum(*p) || *p == '_')) p++;
            push_token(tokens, TOKEN_IDENTIFIER, str, start, p);
            start = p;
        }
        else if (*p == '"') {   /* double quoted string */
            p++;                /* step over double quote */
            while(*p) {
                if (*p == '\\') { /* backslash escape */
                    p++;          /* step over backslash */
                    if (*p) {
                        p++;      /* step over escaped character */
                    } else {
                        break;    /* backslash at end of string, stop */
                    }
                }
                if (*p == '\"') break; /* terminating quote delimiter */
                p++;                   /* keep scanning */
            }
            if (*p != '\"') {
                *error = apr_psprintf(pool,
                                      "unterminated string beginning at "
                                      "position %" APR_SIZE_T_FMT " in \"%s\"",
                                      start-str, str);
                break;
            }
            p++;
            push_token(tokens, TOKEN_DBL_QUOTE_STRING, str, start, p);
            start = p;
        }
        else if (*p == '=') {   /* equals */
            p++;
            push_token(tokens, TOKEN_EQUAL, str, start, p);
            start = p;
        }
        else if (*p == ',') {   /* comma */
            p++;
            push_token(tokens, TOKEN_COMMA, str, start, p);
            start = p;
        }
        else if (*p == ';') {   /* semicolon */
            p++;
            push_token(tokens, TOKEN_SEMICOLON, str, start, p);
            start = p;
        }
        else {                  /* unrecognized token */
            *error = apr_psprintf(pool,
                                  "unknown token at "
                                  "position %" APR_SIZE_T_FMT " in string \"%s\"",
                                  p-str, str);
            break;
        }
    }

    return tokens;
}
