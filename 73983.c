static void cmd_getquota(const char *tag, const char *name)
{
    int r;
    char quotarootbuf[MAX_MAILBOX_BUFFER];
    mbentry_t *mbentry = NULL;
    struct quota q;
    char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);

    quota_init(&q, intname);

    imapd_check(NULL, 0);

    if (!imapd_userisadmin && !imapd_userisproxyadmin) {
        r = IMAP_PERMISSION_DENIED;
        goto done;
    }

    r = mlookup(NULL, NULL, intname, &mbentry);
    if (!r && (mbentry->mbtype & MBTYPE_REMOTE)) {
        /* remote mailbox */

        snprintf(quotarootbuf, sizeof(quotarootbuf), "%s.", intname);

        r = mboxlist_allmbox(quotarootbuf, quota_cb, (void *)mbentry->server, 0);
        if (r) goto done;

        struct backend *s;
        s = proxy_findserver(mbentry->server, &imap_protocol,
                             proxy_userid, &backend_cached,
                             &backend_current, &backend_inbox, imapd_in);
        if (!s) {
            r = IMAP_SERVER_UNAVAILABLE;
            goto done;
        }

        imapd_check(s, 0);

        prot_printf(s->out, "%s Getquota {" SIZE_T_FMT "+}\r\n%s\r\n",
                    tag, strlen(name), name);
        pipe_including_tag(s, tag, 0);

        goto done;
    }

    /* local mailbox */

    r = quota_read(&q, NULL, 0);
    if (r) goto done;

    prot_printf(imapd_out, "* QUOTA ");
    prot_printastring(imapd_out, name);
    prot_printf(imapd_out, " ");
    print_quota_used(imapd_out, &q);
    prot_printf(imapd_out, "\r\n");

    prot_printf(imapd_out, "%s OK %s\r\n", tag,
                error_message(IMAP_OK_COMPLETED));

 done:
    if (r) prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
    mboxlist_entry_free(&mbentry);
    quota_free(&q);
    free(intname);
}
