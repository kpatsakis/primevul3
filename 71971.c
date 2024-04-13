push_token(apr_array_header_t *tokens, TokenType type, const char *str,
           const char *start, const char *end)
{
    apr_size_t offset = start - str;
    Token *token = apr_array_push(tokens);

    if (type == TOKEN_DBL_QUOTE_STRING) {
        /* do not include quotes in token value */
        start++; end--;
    }

    token->type = type;
    token->len = end - start;
    token->offset = offset;
    token->str = apr_pstrmemdup(tokens->pool, start, token->len);

    if (type == TOKEN_DBL_QUOTE_STRING) {
        /*
         * The original HTTP 1.1 spec was ambiguous with respect to
         * backslash quoting inside double quoted strings. This has since
         * been resolved in this errata:
         *
         * http://greenbytes.de/tech/webdav/draft-ietf-httpbis-p1-messaging-16.html#rfc.section.3.2.3
         *
         * Which states:
         *
         * Recipients that process the value of the quoted-string MUST
         * handle a quoted-pair as if it were replaced by the octet
         * following the backslash.
         *
         * Senders SHOULD NOT escape octets in quoted-strings that do not
         * require escaping (i.e., other than DQUOTE and the backslash
         * octet).
         */
        char *p, *t;

        for (p = token->str; *p; p++) {
            if (p[0] == '\\' && p[1]) {
                /*
                 * Found backslash with following character.
                 * Move rest of string down 1 character.
                 */
                for (t = p; *t; t++) {
                    t[0] = t[1];
                }
                token->len--;
            }
        }
    }
}
