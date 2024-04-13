static void autocreate_inbox(void)
{
    if (imapd_userisadmin) return;
    if (imapd_userisproxyadmin) return;

    if (config_getint(IMAPOPT_AUTOCREATE_QUOTA) >= 0) {
        char *inboxname = mboxname_user_mbox(imapd_userid, NULL);
        int r = mboxlist_lookup(inboxname, NULL, NULL);
        free(inboxname);
        if (r != IMAP_MAILBOX_NONEXISTENT) return;
        autocreate_user(&imapd_namespace, imapd_userid);
    }
}
