is_token(apr_array_header_t *tokens, apr_size_t index, TokenType type, const char *value)
{
    if (index >= tokens->nelts) {
        return false;
    }

    Token token = APR_ARRAY_IDX(tokens, index, Token);

    if (token.type != type) {
        return false;
    }

    if (value) {
        if (!g_str_equal(token.str, value)) {
            return false;
        }
    }

    return true;
}
