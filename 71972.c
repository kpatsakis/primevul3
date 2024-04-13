token_type_str(TokenType token_type)
{
    switch(token_type) {
    case TOKEN_WHITESPACE:       return "WHITESPACE";
    case TOKEN_SEMICOLON:        return "SEMICOLON";
    case TOKEN_COMMA:            return "COMMA";
    case TOKEN_EQUAL:            return "EQUAL";
    case TOKEN_IDENTIFIER:       return "IDENTIFIER";
    case TOKEN_DBL_QUOTE_STRING: return "DBL_QUOTE_STRING";
    default:                     return "unknown";
    }
}
