static void cmd_status(char *tag, char *name)
{
    int c;
    unsigned statusitems = 0;
    const char *errstr = "Bad status string";
    mbentry_t *mbentry = NULL;
    struct statusdata sdata = STATUSDATA_INIT;
    int r = 0;

    char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);
    r = mlookup(tag, name, intname, &mbentry);
    if (r == IMAP_MAILBOX_MOVED) {
        /* Eat the argument */
        eatline(imapd_in, prot_getc(imapd_in));
        free(intname);
        return;
    }

    if (!r && (mbentry->mbtype & MBTYPE_REMOTE)) {
        /* remote mailbox */

        if (supports_referrals
            && config_getswitch(IMAPOPT_PROXYD_ALLOW_STATUS_REFERRAL)) {
            imapd_refer(tag, mbentry->server, name);
            /* Eat the argument */
            eatline(imapd_in, prot_getc(imapd_in));
        }
        else {
            struct backend *s;

            s = proxy_findserver(mbentry->server, &imap_protocol,
                                 proxy_userid, &backend_cached,
                                 &backend_current, &backend_inbox, imapd_in);
            if (!s) r = IMAP_SERVER_UNAVAILABLE;

            imapd_check(s, 0);

            if (!r) {
                prot_printf(s->out, "%s Status {" SIZE_T_FMT "+}\r\n%s ", tag,
                            strlen(name), name);
                if (!pipe_command(s, 65536)) {
                    pipe_including_tag(s, tag, 0);
                }
            } else {
                eatline(imapd_in, prot_getc(imapd_in));
                prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
            }
        }

        goto done;
    }

    /* local mailbox */

    imapd_check(NULL, 0);

    c = parse_statusitems(&statusitems, &errstr);
    if (c == EOF) {
        prot_printf(imapd_out, "%s BAD %s\r\n", tag, errstr);
        goto done;
    }

    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out,
                    "%s BAD Unexpected extra arguments to Status\r\n", tag);
        eatline(imapd_in, c);
        goto done;
    }

    /* check permissions */
    if (!r) {
        int myrights = cyrus_acl_myrights(imapd_authstate, mbentry->acl);

        if (!(myrights & ACL_READ)) {
            r = (imapd_userisadmin || (myrights & ACL_LOOKUP)) ?
                IMAP_PERMISSION_DENIED : IMAP_MAILBOX_NONEXISTENT;
        }
    }

    if (!r) r = imapd_statusdata(intname, statusitems, &sdata);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag,
                    error_message(r));
    }
    else {
        print_statusline(name, statusitems, &sdata);
        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
    }

 done:
    if (global_conversations) {
        conversations_abort(&global_conversations);
        global_conversations = NULL;
    }
    mboxlist_entry_free(&mbentry);
    free(intname);
    return;
}
