static int burl_is_unreserved (const int c)
{
    return (light_isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~');
}
