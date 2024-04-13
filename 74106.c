static int xfer_localcreate(struct xfer_header *xfer)
{
    struct xfer_item *item;
    int r;

    syslog(LOG_INFO, "XFER: creating mailboxes on destination");

    for (item = xfer->items; item; item = item->next) {
        if (xfer->topart) {
            /* need to send partition as an atom */
            prot_printf(xfer->be->out, "LC1 LOCALCREATE {" SIZE_T_FMT "+}\r\n%s %s\r\n",
                        strlen(item->extname), item->extname, xfer->topart);
        } else {
            prot_printf(xfer->be->out, "LC1 LOCALCREATE {" SIZE_T_FMT "+}\r\n%s\r\n",
                        strlen(item->extname), item->extname);
        }
        r = getresult(xfer->be->in, "LC1");
        if (r) {
            syslog(LOG_ERR, "Could not move mailbox: %s, LOCALCREATE failed",
                   item->mbentry->name);
            return r;
        }

        item->state = XFER_REMOTE_CREATED;
    }

    return 0;
}
