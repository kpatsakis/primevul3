static void canonical_list_patterns(const char *reference,
                                    strarray_t *patterns)
{
    static int ignorereference = 0;
    int i;

    /* Ignore the reference argument?
       (the behavior in 1.5.10 & older) */
    if (ignorereference == 0)
        ignorereference = config_getswitch(IMAPOPT_IGNOREREFERENCE);

    for (i = 0 ; i < patterns->count ; i++) {
        char *p = patterns->data[i];
        if (!ignorereference || p[0] == imapd_namespace.hier_sep) {
            strarray_setm(patterns, i,
                          canonical_list_pattern(reference, p));
            p = patterns->data[i];
        }
    }
}
