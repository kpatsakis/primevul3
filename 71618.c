int burl_normalize (buffer *b, buffer *t, int flags)
{
    int qs;

  #if defined(__WIN32) || defined(__CYGWIN__)
    /* Windows and Cygwin treat '\\' as '/' if '\\' is present in path;
     * convert to '/' for consistency before percent-encoding
     * normalization which will convert '\\' to "%5C" in the URL.
     * (Clients still should not be sending '\\' unencoded in requests.) */
    if (flags & HTTP_PARSEOPT_URL_NORMALIZE_PATH_BACKSLASH_TRANS) {
        for (char *p = b->ptr; *p != '?' && *p != '\0'; ++p) {
            if (*p == '\\') *p = '/';
        }
    }
  #endif

    qs = (flags & HTTP_PARSEOPT_URL_NORMALIZE_REQUIRED)
      ? burl_normalize_basic_required(b, t)
      : burl_normalize_basic_unreserved(b, t);
    if (-2 == qs) return -2;

    if (flags & HTTP_PARSEOPT_URL_NORMALIZE_CTRLS_REJECT) {
        if (burl_contains_ctrls(b)) return -2;
    }

    if (flags & (HTTP_PARSEOPT_URL_NORMALIZE_PATH_2F_DECODE
                |HTTP_PARSEOPT_URL_NORMALIZE_PATH_2F_REJECT)) {
        qs = burl_normalize_2F_to_slash(b, qs, flags);
        if (-2 == qs) return -2;
    }

    if (flags & (HTTP_PARSEOPT_URL_NORMALIZE_PATH_DOTSEG_REMOVE
                |HTTP_PARSEOPT_URL_NORMALIZE_PATH_DOTSEG_REJECT)) {
        qs = burl_normalize_path(b, t, qs, flags);
        if (-2 == qs) return -2;
    }

    if (flags & HTTP_PARSEOPT_URL_NORMALIZE_QUERY_20_PLUS) {
        if (qs >= 0) burl_normalize_qs20_to_plus(b, qs);
    }

    return qs;
}
