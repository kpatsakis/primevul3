static int xfer_addusermbox(const mbentry_t *mbentry, void *rock)
{
    struct xfer_header *xfer = (struct xfer_header *)rock;

    /* Skip remote mailbox */
    if (mbentry->mbtype & MBTYPE_REMOTE)
        return 0;

    struct xfer_item *item = xzmalloc(sizeof(struct xfer_item));

    int r = mboxlist_lookup(mbentry->name, &item->mbentry, 0);
    if (r) return r;
    char *extname = mboxname_to_external(item->mbentry->name, &imapd_namespace, imapd_userid);
    strncpy(item->extname, extname, sizeof(item->extname));
    free(extname);
    item->mailbox = NULL;
    item->state = 0;

    /* and link on to the list (reverse order) */
    item->next = xfer->items;
    xfer->items = item;

    return 0;
}
