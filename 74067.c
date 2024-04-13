static int parse_metadata_store_data(const char *tag,
                                     struct entryattlist **entryatts)
{
    int c;
    const char *name;
    const char *att;
    static struct buf entry, value;
    struct attvaluelist *attvalues = NULL;
    struct entryattlist *entryp;
    int need_add;

    *entryatts = NULL;

    c = prot_getc(imapd_in);
    if (c != '(') {
        prot_printf(imapd_out,
                    "%s BAD Missing metadata entry list\r\n", tag);
        goto baddata;
    }

    do {
        /* get entry */
        c = getastring(imapd_in, imapd_out, &entry);
        if (c != ' ') {
            prot_printf(imapd_out,
                        "%s BAD Missing metadata entry\r\n", tag);
            goto baddata;
        }
        lcase(entry.s);

        /* get value */
        c = getbnstring(imapd_in, imapd_out, &value);
        if (c == EOF) {
            prot_printf(imapd_out,
                        "%s BAD Missing metadata value\r\n", tag);
            goto baddata;
        }

        if (!strncmp(entry.s, "/private", 8) &&
            (entry.s[8] == '\0' || entry.s[8] == '/')) {
            att = "value.priv";
            name = entry.s + 8;
        }
        else if (!strncmp(entry.s, "/shared", 7) &&
                 (entry.s[7] == '\0' || entry.s[7] == '/')) {
            att = "value.shared";
            name = entry.s + 7;
        }
        else {
            prot_printf(imapd_out,
                        "%s BAD entry must begin with /shared or /private\r\n",
                        tag);
            goto baddata;
        }

        need_add = 1;
        for (entryp = *entryatts; entryp; entryp = entryp->next) {
            if (strcmp(entryp->entry, name)) continue;
            /* it's a match, have to append! */
            appendattvalue(&entryp->attvalues, att, &value);
            need_add = 0;
            break;
        }
        if (need_add) {
            appendattvalue(&attvalues, att, &value);
            appendentryatt(entryatts, name, attvalues);
            attvalues = NULL;
        }
    } while (c == ' ');

    if (c != ')') {
        prot_printf(imapd_out,
                    "%s BAD Missing close paren in annotation entry list \r\n",
                    tag);
        goto baddata;
    }

    c = prot_getc(imapd_in);

    return c;

  baddata:
    if (attvalues) freeattvalues(attvalues);
    if (c != EOF) prot_ungetc(c, imapd_in);
    return EOF;
}
