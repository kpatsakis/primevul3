static int burl_normalize_2F_to_slash (buffer *b, int qs, int flags)
{
    /*("%2F" must already have been uppercased during normalization)*/
    const char * const s = b->ptr;
    const int used = qs < 0 ? (int)buffer_string_length(b) : qs;
    for (int i = 0; i < used; ++i) {
        if (s[i] == '%' && s[i+1] == '2' && s[i+2] == 'F') {
            return (flags & HTTP_PARSEOPT_URL_NORMALIZE_PATH_2F_DECODE)
              ? burl_normalize_2F_to_slash_fix(b, qs, i)
              : -2; /*(flags & HTTP_PARSEOPT_URL_NORMALIZE_PATH_2F_REJECT)*/
        }
    }
    return qs;
}
