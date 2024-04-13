static int get_snippetargs(struct snippetargs **sap)
{
    int c;
    struct snippetargs **prevp = sap;
    struct snippetargs *sa = NULL;
    struct buf arg = BUF_INITIALIZER;
    uint32_t uid;
    char *intname = NULL;

    c = prot_getc(imapd_in);
    if (c != '(') goto syntax_error;

    for (;;) {
        c = prot_getc(imapd_in);
        if (c == ')') break;
        if (c != '(') goto syntax_error;

        c = getastring(imapd_in, imapd_out, &arg);
        if (c != ' ') goto syntax_error;

        intname = mboxname_from_external(buf_cstring(&arg), &imapd_namespace, imapd_userid);

        /* allocate a new snippetargs */
        sa = xzmalloc(sizeof(struct snippetargs));
        sa->mboxname = xstrdup(intname);
        /* append to the list */
        *prevp = sa;
        prevp = &sa->next;

        c = getuint32(imapd_in, &sa->uidvalidity);
        if (c != ' ') goto syntax_error;

        c = prot_getc(imapd_in);
        if (c != '(') break;
        for (;;) {
            c = getuint32(imapd_in, &uid);
            if (c != ' ' && c != ')') goto syntax_error;
            if (sa->uids.count + 1 > sa->uids.alloc) {
                sa->uids.alloc += 64;
                sa->uids.data = xrealloc(sa->uids.data,
                                         sizeof(uint32_t) * sa->uids.alloc);
            }
            sa->uids.data[sa->uids.count++] = uid;
            if (c == ')') break;
        }

        c = prot_getc(imapd_in);
        if (c != ')') goto syntax_error;
    }

    c = prot_getc(imapd_in);
    if (c != ' ') goto syntax_error;

out:
    free(intname);
    buf_free(&arg);
    return c;

syntax_error:
    free_snippetargs(sap);
    c = EOF;
    goto out;
}
