static int mlookup(const char *tag, const char *ext_name,
            const char *name, mbentry_t **mbentryptr)
{
    int r;
    mbentry_t *mbentry = NULL;

    r = mboxlist_lookup(name, &mbentry, NULL);
    if ((r == IMAP_MAILBOX_NONEXISTENT || (!r && (mbentry->mbtype & MBTYPE_RESERVE))) &&
        config_mupdate_server) {
        /* It is not currently active, make sure we have the most recent
         * copy of the database */
        kick_mupdate();
        mboxlist_entry_free(&mbentry);
        r = mboxlist_lookup(name, &mbentry, NULL);
    }

    if (r) goto done;

    if (mbentry->mbtype & MBTYPE_RESERVE) {
        r = IMAP_MAILBOX_RESERVED;
    }
    else if (mbentry->mbtype & MBTYPE_DELETED) {
        r = IMAP_MAILBOX_NONEXISTENT;
    }
    else if (mbentry->mbtype & MBTYPE_MOVING) {
        /* do we have rights on the mailbox? */
        if (!imapd_userisadmin &&
           (!mbentry->acl || !(cyrus_acl_myrights(imapd_authstate, mbentry->acl) & ACL_LOOKUP))) {
            r = IMAP_MAILBOX_NONEXISTENT;
        } else if (tag && ext_name && mbentry->server) {
            imapd_refer(tag, mbentry->server, ext_name);
            r = IMAP_MAILBOX_MOVED;
        } else if (config_mupdate_server) {
            r = IMAP_SERVER_UNAVAILABLE;
        } else {
            r = IMAP_MAILBOX_NOTSUPPORTED;
        }
    }

done:
    if (r) mboxlist_entry_free(&mbentry);
    else if (mbentryptr) *mbentryptr = mbentry;
    else mboxlist_entry_free(&mbentry); /* we don't actually want it! */

    return r;
}
