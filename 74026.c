static void cmd_xwarmup(const char *tag)
{
    const char *cmd = "Xwarmup";
    clock_t start = clock();
    char mytime[100];
    struct buf arg = BUF_INITIALIZER;
    int warmup_flags = 0;
    struct seqset *uids = NULL;
    /* We deal with the mboxlist API instead of the index_state API or
     * mailbox API to avoid the overhead of index_open(), which will
     * block while reading all the cyrus.index...we want to be
     * non-blocking */
    struct mboxlist_entry *mbentry = NULL;
    int myrights;
    int c, r = 0;
    char *intname = NULL;

    /* parse arguments: expect <mboxname> '('<warmup-items>')' */

    c = getastring(imapd_in, imapd_out, &arg);
    if (c != ' ') {
syntax_error:
        prot_printf(imapd_out, "%s BAD syntax error in %s\r\n", tag, cmd);
        eatline(imapd_in, c);
        goto out_noprint;
    }

    intname = mboxname_from_external(arg.s, &imapd_namespace, imapd_userid);
    r = mboxlist_lookup(intname, &mbentry, NULL);
    if (r) goto out;

    /* Do a permissions check to avoid server DoS opportunity.  But we
     * only need read permission to warmup a mailbox.  Also, be careful
     * to avoid telling the client about the existance of mailboxes to
     * which he doesn't have LOOKUP rights. */
    r = IMAP_PERMISSION_DENIED;
    myrights = (mbentry->acl ? cyrus_acl_myrights(imapd_authstate, mbentry->acl) : 0);
    if (imapd_userisadmin)
        r = 0;
    else if (!(myrights & ACL_LOOKUP))
        r = IMAP_MAILBOX_NONEXISTENT;
    else if (myrights & ACL_READ)
        r = 0;
    if (r) goto out;

    if (mbentry->mbtype & MBTYPE_REMOTE) {
        /* remote mailbox */
        struct backend *be;

        be = proxy_findserver(mbentry->server, &imap_protocol,
                              proxy_userid, &backend_cached,
                              &backend_current, &backend_inbox, imapd_in);
        if (!be) {
            r = IMAP_SERVER_UNAVAILABLE;
            goto out;
        }

        prot_printf(be->out, "%s %s %s ", tag, cmd, arg.s);
        if (!pipe_command(backend_current, 65536)) {
            pipe_including_tag(backend_current, tag, 0);
        }
        goto out;
    }
    /* local mailbox */

    /* parse the arguments after the mailbox */

    c = prot_getc(imapd_in);
    if (c != '(') goto syntax_error;

    for (;;) {
        c = getword(imapd_in, &arg);
        if (arg.len) {
            if (!strcasecmp(arg.s, "index"))
                warmup_flags |= WARMUP_INDEX;
            else if (!strcasecmp(arg.s, "conversations"))
                warmup_flags |= WARMUP_CONVERSATIONS;
            else if (!strcasecmp(arg.s, "annotations"))
                warmup_flags |= WARMUP_ANNOTATIONS;
            else if (!strcasecmp(arg.s, "folderstatus"))
                warmup_flags |= WARMUP_FOLDERSTATUS;
            else if (!strcasecmp(arg.s, "search"))
                warmup_flags |= WARMUP_SEARCH;
            else if (!strcasecmp(arg.s, "uids")) {
                if (c != ' ') goto syntax_error;
                c = getword(imapd_in, &arg);
                if (c == EOF) goto syntax_error;
                if (!imparse_issequence(arg.s)) goto syntax_error;
                uids = seqset_parse(arg.s, NULL, /*maxval*/0);
                if (!uids) goto syntax_error;
            }
            else if (!strcasecmp(arg.s, "all"))
                warmup_flags |= WARMUP_ALL;
            else
                goto syntax_error;
        }
        if (c == ')')
            break;
        if (c != ' ') goto syntax_error;
    }

    /* we're expecting no more arguments */
    c = prot_getc(imapd_in);
    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') goto syntax_error;

    r = index_warmup(mbentry, warmup_flags, uids);

out:
    snprintf(mytime, sizeof(mytime), "%2.3f",
             (clock() - start) / (double) CLOCKS_PER_SEC);

    if (r)
        prot_printf(imapd_out, "%s NO %s (%s sec)\r\n", tag,
                    error_message(r), mytime);
    else
        prot_printf(imapd_out, "%s OK %s (%s sec)\r\n", tag,
                    error_message(IMAP_OK_COMPLETED), mytime);

out_noprint:
    mboxlist_entry_free(&mbentry);
    free(intname);
    buf_free(&arg);
    if (uids) seqset_free(uids);
}
