bool am_parse_paos_header(request_rec *r, const char *header,
                             ECPServiceOptions *options_return)
{
    bool result = false;
    ECPServiceOptions options = 0;
    apr_array_header_t *tokens;
    apr_size_t i;
    char *error;

    AM_LOG_RERROR(APLOG_MARK, APLOG_DEBUG, 0, r,
                  "PAOS header: \"%s\"", header);

    tokens = tokenize(r->pool, header, true, &error);

#ifdef DEBUG
    dump_tokens(r, tokens);
#endif

    if (error) {
        AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r, "%s", error);
        goto cleanup;
    }

    /* Header must begin with "ver=xxx" where xxx is paos version */
    if (!is_token(tokens, 0, TOKEN_IDENTIFIER, "ver") ||
        !is_token(tokens, 1, TOKEN_EQUAL, NULL) ||
        !is_token(tokens, 2, TOKEN_DBL_QUOTE_STRING, LASSO_PAOS_HREF)) {
        AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                      "invalid PAOS header, "
                      "expected header to begin with ver=\"%s\", "
                      "actual header=\"%s\"",
                      LASSO_PAOS_HREF, header);
        goto cleanup;
    }

    /* Next is the service value, separated from the version by a semicolon */
    if (!is_token(tokens, 3, TOKEN_SEMICOLON, NULL)) {
        AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                     "invalid PAOS header, "
                     "expected semicolon after PAOS version "
                     "but found %s in header=\"%s\"",
                      parse_error_msg(tokens, 3),
                      header);
        goto cleanup;
    }

    if (!is_token(tokens, 4, TOKEN_DBL_QUOTE_STRING, LASSO_ECP_HREF)) {
        AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                      "invalid PAOS header, "
                      "expected service token to be \"%s\", "
                      "but found %s in header=\"%s\"",
                      LASSO_ECP_HREF,
                      parse_error_msg(tokens, 4),
                      header);
        goto cleanup;
    }

    /* After the service value there may be optional flags separated by commas */

    if (tokens->nelts == 5) {    /* no options */
        result = true;
        goto cleanup;
    }

    /* More tokens after the service value, must be options, iterate over them */
    for (i = 5; i < tokens->nelts; i++) {
        if (!is_token(tokens, i, TOKEN_COMMA, NULL)) {
            AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                          "invalid PAOS header, "
                          "expected comma after PAOS service "
                          "but found %s in header=\"%s\"",
                          parse_error_msg(tokens, i),
                          header);
            goto cleanup;
        }

        if (++i > tokens->nelts) {
            AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                          "invalid PAOS header, "
                          "expected option after comma "
                          "in header=\"%s\"",
                          header);
            goto cleanup;
        }

        Token token = APR_ARRAY_IDX(tokens, i, Token);

        if (token.type != TOKEN_DBL_QUOTE_STRING) {
            AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                          "invalid PAOS header, "
                          "expected quoted string after comma "
                          "but found %s in header=\"%s\"",
                          parse_error_msg(tokens, i),
                          header);
            goto cleanup;
        }

        /* Have an option string, convert it to a bit flag */
        const char *value = token.str;

        if (g_str_equal(value, LASSO_SAML_EXT_CHANNEL_BINDING)) {
            options |= ECP_SERVICE_OPTION_CHANNEL_BINDING;
        } else if (g_str_equal(value, LASSO_SAML2_CONFIRMATION_METHOD_HOLDER_OF_KEY)) {
            options |= ECP_SERVICE_OPTION_HOLDER_OF_KEY;
        } else if (g_str_equal(value, LASSO_SAML2_ECP_PROFILE_WANT_AUTHN_SIGNED)) {
            options |= ECP_SERVICE_OPTION_WANT_AUTHN_SIGNED;
        } else if (g_str_equal(value, LASSO_SAML2_CONDITIONS_DELEGATION)) {
            options |= ECP_SERVICE_OPTION_DELEGATION;
        } else {
            AM_LOG_RERROR(APLOG_MARK, APLOG_WARNING, 0, r,
                          "Unknown PAOS service option = \"%s\"",
                          value);
            goto cleanup;
        }
    }

    result = true;

 cleanup:
    if (options_return) {
        *options_return = options;
    }
    return result;

}
