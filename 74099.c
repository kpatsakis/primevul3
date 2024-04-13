static void xfer_cleanup(struct xfer_header *xfer)
{
    struct xfer_item *item, *next;

    /* remove items */
    item = xfer->items;
    while (item) {
        next = item->next;
        mboxlist_entry_free(&item->mbentry);
        free(item);
        item = next;
    }
    xfer->items = NULL;

    free(xfer->topart);
    free(xfer->userid);
    xfer->topart = xfer->userid = NULL;

    seen_close(&xfer->seendb);
    xfer->seendb = NULL;
}
