void cmd_setquota(const char *tag, const char *quotaroot)
{
    quota_t newquotas[QUOTA_NUMRESOURCES];
    int res;
    int c;
    int force = 0;
    static struct buf arg;
    int r;
    mbentry_t *mbentry = NULL;
    char *intname = NULL;

    if (!imapd_userisadmin && !imapd_userisproxyadmin) {
        /* need to allow proxies so that mailbox moves can set initial quota
         * roots */
        r = IMAP_PERMISSION_DENIED;
        goto out;
    }

    /* are we forcing the creation of a quotaroot by having a leading +? */
    if (quotaroot[0] == '+') {
        force = 1;
        quotaroot++;
    }

    intname = mboxname_from_external(quotaroot, &imapd_namespace, imapd_userid);

    r = mlookup(NULL, NULL, intname, &mbentry);
    if (r == IMAP_MAILBOX_NONEXISTENT)
        r = 0;      /* will create a quotaroot anyway */
    if (r)
        goto out;

    if (mbentry && (mbentry->mbtype & MBTYPE_REMOTE)) {
        /* remote mailbox */
        struct backend *s;
        char quotarootbuf[MAX_MAILBOX_BUFFER];

        snprintf(quotarootbuf, sizeof(quotarootbuf), "%s.", intname);

        r = mboxlist_allmbox(quotarootbuf, quota_cb, (void *)mbentry->server, 0);
        if (r)
            goto out;

        imapd_check(NULL, 0);

        s = proxy_findserver(mbentry->server, &imap_protocol,
                             proxy_userid, &backend_cached,
                             &backend_current, &backend_inbox, imapd_in);
        if (!s) {
            r = IMAP_SERVER_UNAVAILABLE;
            goto out;
        }

        imapd_check(s, 0);

        prot_printf(s->out, "%s Setquota ", tag);
        prot_printstring(s->out, quotaroot);
        prot_putc(' ', s->out);
        pipe_command(s, 0);
        pipe_including_tag(s, tag, 0);

        free(intname);
        return;

    }
    mboxlist_entry_free(&mbentry);

    /* local mailbox */

    /* Now parse the arguments as a setquota_list */
    c = prot_getc(imapd_in);
    if (c != '(') goto badlist;

    for (res = 0 ; res < QUOTA_NUMRESOURCES ; res++)
        newquotas[res] = QUOTA_UNLIMITED;

    for (;;) {
        /* XXX - limit is actually stored in an int value */
        int32_t limit = 0;

        c = getword(imapd_in, &arg);
        if ((c == ')') && !arg.s[0]) break;
        if (c != ' ') goto badlist;
        res = quota_name_to_resource(arg.s);
        if (res < 0) {
            r = IMAP_UNSUPPORTED_QUOTA;
            goto out;
        }

        c = getsint32(imapd_in, &limit);
        /* note: we accept >= 0 according to rfc2087,
         * and also -1 to fix Bug #3559 */
        if (limit < -1) goto badlist;
        newquotas[res] = limit;
        if (c == ')') break;
        else if (c != ' ') goto badlist;
    }
    c = prot_getc(imapd_in);
    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out, "%s BAD Unexpected extra arguments to SETQUOTA\r\n", tag);
        eatline(imapd_in, c);
        return;
    }

    r = mboxlist_setquotas(intname, newquotas, force);

    imapd_check(NULL, 0);
out:
    mboxlist_entry_free(&mbentry);
    free(intname);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        return;
    }

    prot_printf(imapd_out, "%s OK %s\r\n", tag,
                error_message(IMAP_OK_COMPLETED));
    return;

 badlist:
    prot_printf(imapd_out, "%s BAD Invalid quota list in Setquota\r\n", tag);
    eatline(imapd_in, c);
    free(intname);
}
