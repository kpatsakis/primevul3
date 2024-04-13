static void cmd_getquotaroot(const char *tag, const char *name)
{
    mbentry_t *mbentry = NULL;
    struct mailbox *mailbox = NULL;
    int myrights;
    int r, doclose = 0;

    char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);
    r = mlookup(tag, name, intname, &mbentry);
    if (r == IMAP_MAILBOX_MOVED) {
        free(intname);
        return;
    }

    if (!r && (mbentry->mbtype & MBTYPE_REMOTE)) {
        /* remote mailbox */
        struct backend *s;

        s = proxy_findserver(mbentry->server, &imap_protocol,
                             proxy_userid, &backend_cached,
                             &backend_current, &backend_inbox, imapd_in);
        if (!s) r = IMAP_SERVER_UNAVAILABLE;

        imapd_check(s, 0);

        if (!r) {
            prot_printf(s->out, "%s Getquotaroot {" SIZE_T_FMT "+}\r\n%s\r\n",
                        tag, strlen(name), name);
            pipe_including_tag(s, tag, 0);
        } else {
            prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        }

        mboxlist_entry_free(&mbentry);
        free(intname);

        return;
    }

    mboxlist_entry_free(&mbentry);

    /* local mailbox */
    if (!r) {
        r = mailbox_open_irl(intname, &mailbox);
        if (!r) {
            doclose = 1;
            myrights = cyrus_acl_myrights(imapd_authstate, mailbox->acl);
        }
    }

    if (!r) {
        if (!imapd_userisadmin && !(myrights & ACL_READ)) {
            r = (myrights & ACL_LOOKUP) ?
              IMAP_PERMISSION_DENIED : IMAP_MAILBOX_NONEXISTENT;
        }
    }

    if (!r) {
        prot_printf(imapd_out, "* QUOTAROOT ");
        prot_printastring(imapd_out, name);
        if (mailbox->quotaroot) {
            struct quota q;
            char *extname = mboxname_to_external(mailbox->quotaroot, &imapd_namespace, imapd_userid);
            prot_printf(imapd_out, " ");
            prot_printastring(imapd_out, extname);
            quota_init(&q, mailbox->quotaroot);
            r = quota_read(&q, NULL, 0);
            if (!r) {
                prot_printf(imapd_out, "\r\n* QUOTA ");
                prot_printastring(imapd_out, extname);
                prot_putc(' ', imapd_out);
                print_quota_used(imapd_out, &q);
            }
            quota_free(&q);
            free(extname);
        }
        prot_printf(imapd_out, "\r\n");
    }

    if (doclose) mailbox_close(&mailbox);

    free(intname);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        return;
    }

    imapd_check(NULL, 0);

    prot_printf(imapd_out, "%s OK %s\r\n", tag,
                error_message(IMAP_OK_COMPLETED));
}
