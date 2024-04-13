static void cmd_delete(char *tag, char *name, int localonly, int force)
{
    int r;
    mbentry_t *mbentry = NULL;
    struct mboxevent *mboxevent = NULL;

    char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);
    r = mlookup(NULL, NULL, intname, &mbentry);

    if (!r && (mbentry->mbtype & MBTYPE_REMOTE)) {
        /* remote mailbox */
        struct backend *s = NULL;
        int res;

        if (supports_referrals) {
            imapd_refer(tag, mbentry->server, name);
            referral_kick = 1;
            mboxlist_entry_free(&mbentry);
            free(intname);
            return;
        }

        s = proxy_findserver(mbentry->server, &imap_protocol,
                             proxy_userid, &backend_cached,
                             &backend_current, &backend_inbox, imapd_in);
        mboxlist_entry_free(&mbentry);
        if (!s) r = IMAP_SERVER_UNAVAILABLE;

        if (!r) {
            prot_printf(s->out, "%s DELETE {" SIZE_T_FMT "+}\r\n%s\r\n",
                        tag, strlen(name), name);
            res = pipe_until_tag(s, tag, 0);

            if (!CAPA(s, CAPA_MUPDATE) && res == PROXY_OK) {
                /* do MUPDATE delete operations */
            }

            /* make sure we've seen the update */
            if (ultraparanoid && res == PROXY_OK) kick_mupdate();
        }

        imapd_check(s, 0);

        if (r) {
            prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        } else {
            /* we're allowed to reference last_result since the noop, if
               sent, went to a different server */
            prot_printf(imapd_out, "%s %s", tag, s->last_result.s);
        }

        free(intname);
        return;
    }
    mboxlist_entry_free(&mbentry);

    mboxevent = mboxevent_new(EVENT_MAILBOX_DELETE);

    /* local mailbox */
    if (!r) {
        if (localonly || !mboxlist_delayed_delete_isenabled()) {
            r = mboxlist_deletemailbox(intname,
                                       imapd_userisadmin || imapd_userisproxyadmin,
                                       imapd_userid, imapd_authstate, mboxevent,
                                       1-force, localonly, 0);
        } else if ((imapd_userisadmin || imapd_userisproxyadmin) &&
                   mboxname_isdeletedmailbox(intname, NULL)) {
            r = mboxlist_deletemailbox(intname,
                                       imapd_userisadmin || imapd_userisproxyadmin,
                                       imapd_userid, imapd_authstate, mboxevent,
                                       0 /* checkacl */, localonly, 0);
        } else {
            r = mboxlist_delayed_deletemailbox(intname,
                                               imapd_userisadmin || imapd_userisproxyadmin,
                                               imapd_userid, imapd_authstate, mboxevent,
                                               1-force, 0, 0);
        }
    }

    /* send a MailboxDelete event notification */
    if (!r)
        mboxevent_notify(&mboxevent);
    mboxevent_free(&mboxevent);

    /* was it a top-level user mailbox? */
    /* localonly deletes are only per-mailbox */
    if (!r && !localonly && mboxname_isusermailbox(intname, 1)) {
        char *userid = mboxname_to_userid(intname);
        if (userid) {
            r = mboxlist_usermboxtree(userid, delmbox, NULL, 0);
            if (!r) r = user_deletedata(userid, 1);
            free(userid);
        }
    }

    if (!r && config_getswitch(IMAPOPT_DELETE_UNSUBSCRIBE)) {
        mboxlist_changesub(intname, imapd_userid, imapd_authstate,
                           /* add */ 0, /* force */ 0, /* notify? */ 1);
    }

    imapd_check(NULL, 0);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
    }
    else {
        if (config_mupdate_server)
            kick_mupdate();

        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
    }
    free(intname);
}
