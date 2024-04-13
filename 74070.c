static int parse_windowargs(const char *tag,
                            struct windowargs **wa,
                            int updates)
{
    struct windowargs windowargs;
    struct buf arg = BUF_INITIALIZER;
    struct buf ext_folder = BUF_INITIALIZER;
    int c;

    memset(&windowargs, 0, sizeof(windowargs));

    c = prot_getc(imapd_in);
    if (c == EOF)
        goto out;
    if (c != '(') {
        /* no window args at all */
        prot_ungetc(c, imapd_in);
        goto out;
    }

    for (;;)
    {
        c = prot_getc(imapd_in);
        if (c == EOF)
            goto out;
        if (c == ')')
            break;  /* end of window args */

        prot_ungetc(c, imapd_in);
        c = getword(imapd_in, &arg);
        if (!arg.len)
            goto syntax_error;

        if (!strcasecmp(arg.s, "CONVERSATIONS"))
            windowargs.conversations = 1;
        else if (!strcasecmp(arg.s, "POSITION")) {
            if (updates)
                goto syntax_error;
            if (c != ' ')
                goto syntax_error;
            c = prot_getc(imapd_in);
            if (c != '(')
                goto syntax_error;
            c = getuint32(imapd_in, &windowargs.position);
            if (c != ' ')
                goto syntax_error;
            c = getuint32(imapd_in, &windowargs.limit);
            if (c != ')')
                goto syntax_error;
            c = prot_getc(imapd_in);
            if (windowargs.position == 0)
                goto syntax_error;
        }
        else if (!strcasecmp(arg.s, "ANCHOR")) {
            if (updates)
                goto syntax_error;
            if (c != ' ')
                goto syntax_error;
            c = prot_getc(imapd_in);
            if (c != '(')
                goto syntax_error;
            c = getuint32(imapd_in, &windowargs.anchor);
            if (c != ' ')
                goto syntax_error;
            c = getuint32(imapd_in, &windowargs.offset);
            if (c != ' ')
                goto syntax_error;
            c = getuint32(imapd_in, &windowargs.limit);
            if (c != ')')
                goto syntax_error;
            c = prot_getc(imapd_in);
            if (windowargs.anchor == 0)
                goto syntax_error;
        }
        else if (!strcasecmp(arg.s, "MULTIANCHOR")) {
            if (updates)
                goto syntax_error;
            if (c != ' ')
                goto syntax_error;
            c = prot_getc(imapd_in);
            if (c != '(')
                goto syntax_error;
            c = getuint32(imapd_in, &windowargs.anchor);
            if (c != ' ')
                goto syntax_error;
            c = getastring(imapd_in, imapd_out, &ext_folder);
            if (c != ' ')
                goto syntax_error;
            c = getuint32(imapd_in, &windowargs.offset);
            if (c != ' ')
                goto syntax_error;
            c = getuint32(imapd_in, &windowargs.limit);
            if (c != ')')
                goto syntax_error;
            c = prot_getc(imapd_in);
            if (windowargs.anchor == 0)
                goto syntax_error;
        }
        else if (!strcasecmp(arg.s, "CHANGEDSINCE")) {
            if (!updates)
                goto syntax_error;
            windowargs.changedsince = 1;
            if (c != ' ')
                goto syntax_error;
            c = prot_getc(imapd_in);
            if (c != '(')
                goto syntax_error;
            c = getmodseq(imapd_in, &windowargs.modseq);
            if (c != ' ')
                goto syntax_error;
            c = getuint32(imapd_in, &windowargs.uidnext);
            if (c != ')')
                goto syntax_error;
            c = prot_getc(imapd_in);
        } else if (!strcasecmp(arg.s, "UPTO")) {
            if (!updates)
                goto syntax_error;
            if (c != ' ')
                goto syntax_error;
            c = prot_getc(imapd_in);
            if (c != '(')
                goto syntax_error;
            c = getuint32(imapd_in, &windowargs.upto);
            if (c != ')')
                goto syntax_error;
            c = prot_getc(imapd_in);

            if (windowargs.upto == 0)
                goto syntax_error;
        }
        else
            goto syntax_error;

        if (c == ')')
            break;
        if (c != ' ')
            goto syntax_error;
    }

    c = prot_getc(imapd_in);
    if (c != ' ')
        goto syntax_error;

out:
    /* these two are mutually exclusive */
    if (windowargs.anchor && windowargs.position)
        goto syntax_error;
    /* changedsince is mandatory for XCONVUPDATES
     * and illegal for XCONVSORT */
    if (!!updates != windowargs.changedsince)
        goto syntax_error;

    if (ext_folder.len) {
        windowargs.anchorfolder = mboxname_from_external(buf_cstring(&ext_folder),
                                                         &imapd_namespace,
                                                         imapd_userid);
    }

    *wa = xmemdup(&windowargs, sizeof(windowargs));
    buf_free(&ext_folder);
    buf_free(&arg);
    return c;

syntax_error:
    free(windowargs.anchorfolder);
    buf_free(&ext_folder);
    prot_printf(imapd_out, "%s BAD Syntax error in window arguments\r\n", tag);
    return EOF;
}
