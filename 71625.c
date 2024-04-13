static void burl_normalize_qs20_to_plus (buffer *b, int qs)
{
    const char * const s = b->ptr;
    const int used = qs < 0 ? 0 : (int)buffer_string_length(b);
    int i;
    if (qs < 0) return;
    for (i = qs+1; i < used; ++i) {
        if (s[i] == '%' && s[i+1] == '2' && s[i+2] == '0') break;
    }
    if (i != used) burl_normalize_qs20_to_plus_fix(b, i);
}
