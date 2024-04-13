static void dump_tokens(request_rec *r, apr_array_header_t *tokens)
{
    apr_size_t i;
    
    for (i = 0; i < tokens->nelts; i++) {
        Token token = APR_ARRAY_IDX(tokens, i, Token);
        AM_LOG_RERROR(APLOG_MARK, APLOG_DEBUG, 0, r,
                      "token[%2zd] %s \"%s\" offset=%lu len=%lu ", i,
                      token_type_str(token.type), token.str,
                      token.offset, token.len);
    }
}
