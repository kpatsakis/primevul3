static void burl_normalize_qs20_to_plus_fix (buffer *b, int i)
{
    char * const s = b->ptr;
    const int used = (int)buffer_string_length(b);
    int j = i;
    for (; i < used; ++i, ++j) {
        s[j] = s[i];
        if (s[i] == '%' && s[i+1] == '2' && s[i+2] == '0') {
            s[j] = '+';
            i+=2;
        }
    }
    buffer_string_set_length(b, j);
}
