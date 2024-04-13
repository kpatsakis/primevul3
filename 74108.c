static int xfer_reactivate(struct xfer_header *xfer)
{
    struct xfer_item *item;
    int r;

    syslog(LOG_INFO, "XFER: reactivating mailboxes");

    if (!mupdate_h) return 0;

    /* 6.5) Kick remote server to correct mupdate entry */
    for (item = xfer->items; item; item = item->next) {
        prot_printf(xfer->be->out, "MP1 MUPDATEPUSH {" SIZE_T_FMT "+}\r\n%s\r\n",
                    strlen(item->extname), item->extname);
        r = getresult(xfer->be->in, "MP1");
        if (r) {
            syslog(LOG_ERR, "MUPDATE: can't activate mailbox entry '%s': %s",
                   item->mbentry->name, error_message(r));
        }
    }

    return 0;
}
