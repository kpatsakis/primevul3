static int burl_normalize_basic_required (buffer *b, buffer *t)
{
    const unsigned char * const s = (unsigned char *)b->ptr;
    const int used = (int)buffer_string_length(b);
    unsigned int n1, n2, x;
    int qs = -1;

    for (int i = 0; i < used; ++i) {
        if (!encoded_chars_http_uri_reqd[s[i]]) {
            if (s[i] == '?' && -1 == qs) qs = i;
        }
        else if (s[i]=='%' && li_cton(s[i+1], n1) && li_cton(s[i+2], n2)
                 && (encoded_chars_http_uri_reqd[(x = (n1 << 4) | n2)]
                     ||(qs < 0 ? (x=='/'||x=='?') : (x=='&'||x=='='||x==';')))){
            if (li_utf8_invalid_byte(x)) qs = -2;
            if (s[i+1] >= 'a') b->ptr[i+1] &= 0xdf; /* uppercase hex */
            if (s[i+2] >= 'a') b->ptr[i+2] &= 0xdf; /* uppercase hex */
            i+=2;
        }
        else if (s[i] == '#') { /* ignore fragment */
            buffer_string_set_length(b, (size_t)i);
            break;
        }
        else {
            qs = burl_normalize_basic_required_fix(b, t, i, qs);
            break;
        }
    }

    return qs;
}
