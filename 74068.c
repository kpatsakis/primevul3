static int parse_metadata_string_or_list(const char *tag,
                                         strarray_t *entries,
                                         int *is_list)
{
    int c;
    static struct buf arg;

    if (is_list) *is_list = 1;

    c = prot_getc(imapd_in);
    if (c == EOF) {
        prot_printf(imapd_out,
                    "%s BAD Missing metadata entry\r\n", tag);
        goto baddata;
    }
    else if (c == '\r') {
        return c;
    }
    else if (c == '(') {
        /* entry list */
        do {
            c = getastring(imapd_in, imapd_out, &arg);
            if (c == EOF) {
                prot_printf(imapd_out,
                            "%s BAD Missing metadata entry\r\n", tag);
                goto baddata;
            }

            /* add the entry to the list */
            strarray_append(entries, arg.s);

        } while (c == ' ');

        if (c != ')') {
            prot_printf(imapd_out,
                        "%s BAD Missing close paren in metadata entry list \r\n",
                        tag);
            goto baddata;
        }

        if (is_list) *is_list = 0;

        c = prot_getc(imapd_in);
    }
    else {
        /* single entry -- add it to the list */
        prot_ungetc(c, imapd_in);
        c = getastring(imapd_in, imapd_out, &arg);
        if (c == EOF) {
            prot_printf(imapd_out,
                        "%s BAD Missing metadata entry\r\n", tag);
            goto baddata;
        }

        strarray_append(entries, arg.s);

        if (!strchr(arg.s, '*') && !strchr(arg.s, '%')) {
            if (is_list) *is_list = 0;
        }
    }

    if (c == ' ' || c == '\r' || c == ')') return c;

  baddata:
    if (c != EOF) prot_ungetc(c, imapd_in);
    return EOF;
}
