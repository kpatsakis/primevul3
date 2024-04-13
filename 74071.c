static int parsecreateargs(struct dlist **extargs)
{
    int c;
    static struct buf arg, val;
    struct dlist *res;
    struct dlist *sub;
    char *p;
    const char *name;

    res = dlist_newkvlist(NULL, "CREATE");

    c = prot_getc(imapd_in);
    if (c == '(') {
        /* new style RFC4466 arguments */
        do {
            c = getword(imapd_in, &arg);
            name = ucase(arg.s);
            if (c != ' ') goto fail;
            c = prot_getc(imapd_in);
            if (c == '(') {
                /* fun - more lists! */
                sub = dlist_newlist(res, name);
                do {
                    c = getword(imapd_in, &val);
                    dlist_setatom(sub, name, val.s);
                } while (c == ' ');
                if (c != ')') goto fail;
                c = prot_getc(imapd_in);
            }
            else {
                prot_ungetc(c, imapd_in);
                c = getword(imapd_in, &val);
                dlist_setatom(res, name, val.s);
            }
        } while (c == ' ');
        if (c != ')') goto fail;
        c = prot_getc(imapd_in);
    }
    else {
        prot_ungetc(c, imapd_in);
        c = getword(imapd_in, &arg);
        if (c == EOF) goto fail;
        p = strchr(arg.s, '!');
        if (p) {
            /* with a server */
            *p = '\0';
            dlist_setatom(res, "SERVER", arg.s);
            dlist_setatom(res, "PARTITION", p+1);
        }
        else {
            dlist_setatom(res, "PARTITION", arg.s);
        }
    }

    *extargs = res;
    return c;

 fail:
    dlist_free(&res);
    return EOF;
}
