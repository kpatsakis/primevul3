static int burl_contains_ctrls (const buffer *b)
{
    const char * const s = b->ptr;
    const int used = (int)buffer_string_length(b);
    for (int i = 0; i < used; ++i) {
        if (s[i] == '%' && (s[i+1] < '2' || (s[i+1] == '7' && s[i+2] == 'F')))
            return 1;
    }
    return 0;
}
