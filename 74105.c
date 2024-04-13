static int xfer_initialsync(struct xfer_header *xfer)
{
    unsigned flags = SYNC_FLAG_LOGGING | SYNC_FLAG_LOCALONLY;
    int r;

    if (xfer->userid) {
        struct xfer_item *item, *next;

        syslog(LOG_INFO, "XFER: initial sync of user %s", xfer->userid);

        r = sync_do_user(xfer->userid, xfer->topart, xfer->be,
                         /*channelp*/NULL, flags);
        if (r) return r;

        /* User moves may take a while, do another non-blocking sync */
        syslog(LOG_INFO, "XFER: second sync of user %s", xfer->userid);

        r = sync_do_user(xfer->userid, xfer->topart, xfer->be,
                         /*channelp*/NULL, flags);
        if (r) return r;

        /* User may have renamed/deleted a mailbox while syncing,
           recreate the submailboxes list */
        for (item = xfer->items; item; item = next) {
            next = item->next;
            mboxlist_entry_free(&item->mbentry);
            free(item);
        }
        xfer->items = NULL;

        r = mboxlist_usermboxtree(xfer->userid, xfer_addusermbox,
                                  xfer, MBOXTREE_DELETED);
    }
    else {
        struct sync_name_list *mboxname_list = sync_name_list_create();

        syslog(LOG_INFO, "XFER: initial sync of mailbox %s",
               xfer->items->mbentry->name);

        sync_name_list_add(mboxname_list, xfer->items->mbentry->name);
        r = sync_do_mailboxes(mboxname_list, xfer->topart, xfer->be,
                              /*channelp*/NULL, flags);
        sync_name_list_free(&mboxname_list);
    }

    return r;
}
