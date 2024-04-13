static int burl_normalize_path (buffer *b, buffer *t, int qs, int flags)
{
    const unsigned char * const s = (unsigned char *)b->ptr;
    const int used = (int)buffer_string_length(b);
    int path_simplify = 0;
    for (int i = 0, len = qs < 0 ? used : qs; i < len; ++i) {
        if (s[i] == '.' && (s[i+1] != '.' || ++i)
            && (s[i+1] == '/' || s[i+1] == '?' || s[i+1] == '\0')) {
            path_simplify = 1;
            break;
        }
        do { ++i; } while (i < len && s[i] != '/');
        if (s[i] == '/' && s[i+1] == '/') { /*(s[len] != '/')*/
            path_simplify = 1;
            break;
        }
    }

    if (path_simplify) {
        if (flags & HTTP_PARSEOPT_URL_NORMALIZE_PATH_DOTSEG_REJECT) return -2;
        if (qs >= 0) {
            buffer_copy_string_len(t, b->ptr+qs, used - qs);
            buffer_string_set_length(b, qs);
        }

        buffer_path_simplify(b, b);

        if (qs >= 0) {
            qs = (int)buffer_string_length(b);
            buffer_append_string_len(b, CONST_BUF_LEN(t));
        }
    }

    return qs;
}
