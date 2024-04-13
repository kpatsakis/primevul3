static void xfer_recover(struct xfer_header *xfer)
{
    struct xfer_item *item;
    int r;

    syslog(LOG_INFO, "XFER: recovering");

    /* Backout any changes - we stop on first untouched mailbox */
    for (item = xfer->items; item && item->state; item = item->next) {
        switch (item->state) {
        case XFER_UNDUMPED:
        case XFER_LOCAL_MOVING:
            /* Unset mailbox as MOVING on local server */
            r = mboxlist_update(item->mbentry, 1);

            if (r) {
                syslog(LOG_ERR,
                       "Could not back out MOVING flag during move of %s (%s)",
                       item->mbentry->name, error_message(r));
            }

        case XFER_REMOTE_CREATED:
            if (!xfer->use_replication) {
                /* Delete remote mailbox */
                prot_printf(xfer->be->out,
                            "LD1 LOCALDELETE {" SIZE_T_FMT "+}\r\n%s\r\n",
                            strlen(item->extname), item->extname);
                r = getresult(xfer->be->in, "LD1");
                if (r) {
                    syslog(LOG_ERR,
                        "Could not back out remote mailbox during move of %s (%s)",
                        item->mbentry->name, error_message(r));
                }
            }

        case XFER_DEACTIVATED:
            /* Tell murder it's back here and active */
            r = xfer_mupdate(1, item->mbentry->name, item->mbentry->partition,
                             config_servername, item->mbentry->acl);
            if (r) {
                syslog(LOG_ERR,
                       "Could not back out mupdate during move of %s (%s)",
                       item->mbentry->name, error_message(r));
            }
        }
    }
}
