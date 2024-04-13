static int delmbox(const mbentry_t *mbentry, void *rock __attribute__((unused)))
{
    int r;

    if (!mboxlist_delayed_delete_isenabled()) {
        r = mboxlist_deletemailbox(mbentry->name,
                                   imapd_userisadmin || imapd_userisproxyadmin,
                                   imapd_userid, imapd_authstate, NULL,
                                   0, 0, 0);
    } else if ((imapd_userisadmin || imapd_userisproxyadmin) &&
               mboxname_isdeletedmailbox(mbentry->name, NULL)) {
        r = mboxlist_deletemailbox(mbentry->name,
                                   imapd_userisadmin || imapd_userisproxyadmin,
                                   imapd_userid, imapd_authstate, NULL,
                                   0, 0, 0);
    } else {
        r = mboxlist_delayed_deletemailbox(mbentry->name,
                                           imapd_userisadmin || imapd_userisproxyadmin,
                                           imapd_userid, imapd_authstate, NULL,
                                           0, 0, 0);
    }

    if (r) {
        prot_printf(imapd_out, "* NO delete %s: %s\r\n",
                    mbentry->name, error_message(r));
    }

    return 0;
}
