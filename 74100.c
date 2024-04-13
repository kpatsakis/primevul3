static int xfer_deactivate(struct xfer_header *xfer)
{
    struct xfer_item *item;
    int r;

    syslog(LOG_INFO, "XFER: deactivating mailboxes");

    /* Step 3: mupdate.DEACTIVATE(mailbox, newserver) */
    for (item = xfer->items; item; item = item->next) {
        r = xfer_mupdate(0, item->mbentry->name, item->mbentry->partition,
                         config_servername, item->mbentry->acl);
        if (r) {
            syslog(LOG_ERR,
                   "Could not move mailbox: %s, MUPDATE DEACTIVATE failed",
                   item->mbentry->name);
            return r;
        }

        item->state = XFER_DEACTIVATED;
    }

    return 0;
}
