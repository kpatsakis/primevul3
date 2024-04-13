static char *canonical_list_pattern(const char *reference, const char *pattern)
{
    int patlen = strlen(pattern);
    int reflen = strlen(reference);

    char *buf = xmalloc(patlen + reflen + 1);
    buf[0] = '\0';

    if (*reference) {
        if (reference[reflen-1] == imapd_namespace.hier_sep &&
                pattern[0] == imapd_namespace.hier_sep)
            --reflen;
        memcpy(buf, reference, reflen);
        buf[reflen] = '\0';
    }
    strcat(buf, pattern);

    return buf;
}
