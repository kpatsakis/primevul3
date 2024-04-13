static int xfer_addmbox(struct findall_data *data, void *rock)
{
    if (!data) return 0;
    struct xfer_list *list = (struct xfer_list *) rock;

    if (!data->mbentry) {
        /* No partial matches */
        return 0;
    }

    if (list->part && strcmp(data->mbentry->partition, list->part)) {
        /* Not on specified partition */
        return 0;
    }

    /* Only add shared mailboxes, targeted user submailboxes, or user INBOXes */
    if (!mbname_localpart(data->mbname) || list->allow_usersubs ||
        (!mbname_isdeleted(data->mbname) && !strarray_size(mbname_boxes(data->mbname)))) {
        const char *extname = mbname_extname(data->mbname, list->ns, list->userid);
        struct xfer_item *mbox = xzmalloc(sizeof(struct xfer_item));

        mbox->mbentry = mboxlist_entry_copy(data->mbentry);
        strncpy(mbox->extname, extname, sizeof(mbox->extname));
        if (mbname_localpart(data->mbname) && !list->allow_usersubs) {
            /* User INBOX */
            mbox->state = XFER_MOVING_USER;
        }

        /* Add link on to the list (reverse order) */
        mbox->next = list->mboxes;
        list->mboxes = mbox;
    }

    return 0;
}
