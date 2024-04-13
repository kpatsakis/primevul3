static int burl_normalize_basic_unreserved_fix (buffer *b, buffer *t, int i, int qs)
{
    int j = i;
    const int used = (int)buffer_string_length(b);
    const unsigned char * const s = (unsigned char *)b->ptr;
    unsigned char * const p =
      (unsigned char *)buffer_string_prepare_copy(t,i+(used-i)*3+1);
    unsigned int n1, n2;
    memcpy(p, s, (size_t)i);
    for (; i < used; ++i, ++j) {
        if (!encoded_chars_http_uri_reqd[s[i]]) {
            if (s[i] == '?' && -1 == qs) qs = j;
            p[j] = s[i];
        }
        else if (s[i]=='%' && li_cton(s[i+1], n1) && li_cton(s[i+2], n2)) {
            const unsigned int x = (n1 << 4) | n2;
            if (burl_is_unreserved(x)) {
                p[j] = x;
            }
            else {
                p[j]   = '%';
                p[++j] = hex_chars_uc[n1]; /*(s[i+1] & 0xdf)*/
                p[++j] = hex_chars_uc[n2]; /*(s[i+2] & 0xdf)*/
                if (li_utf8_invalid_byte(x)) qs = -2;
            }
            i+=2;
        }
        else if (s[i] == '#') break; /* ignore fragment */
        else {
            p[j]   = '%';
            p[++j] = hex_chars_uc[(s[i] >> 4) & 0xF];
            p[++j] = hex_chars_uc[s[i] & 0xF];
            if (li_utf8_invalid_byte(s[i])) qs = -2;
        }
    }
    buffer_commit(t, (size_t)j);
    buffer_copy_buffer(b, t);
    return qs;
}
