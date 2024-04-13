static int subscribed_cb(struct findall_data *data, void *rockp)
{
    struct list_rock *rock = (struct list_rock *)rockp;
    if (!data) {
        perform_output(NULL, NULL, rock);
        return 0;
    }
    size_t last_len = (rock->last_name ? strlen(rock->last_name) : 0);
    const char *extname = data->extname;
    int last_name_is_ancestor =
        rock->last_name
        && strlen(extname) >= last_len
        && extname[last_len] == imapd_namespace.hier_sep
        && !memcmp(rock->last_name, extname, last_len);

    list_callback_calls++;

    if (last_name_is_ancestor ||
        (rock->last_name && !data->mbname && !strcmp(rock->last_name, extname)))
        rock->last_attributes |= MBOX_ATTRIBUTE_HASCHILDREN;

    if (data->mbname) { /* exact match */
        mbentry_t *mbentry = NULL;
        mboxlist_lookup(mbname_intname(data->mbname), &mbentry, NULL);
        perform_output(extname, mbentry, rock);
        mboxlist_entry_free(&mbentry);

        rock->last_attributes |= MBOX_ATTRIBUTE_SUBSCRIBED;
        if (mboxlist_lookup(mbname_intname(data->mbname), NULL, NULL))
            rock->last_attributes |= MBOX_ATTRIBUTE_NONEXISTENT;
        if (data->mb_category == MBNAME_ALTINBOX)
            rock->last_attributes |= MBOX_ATTRIBUTE_NOINFERIORS;
    }
    else if (rock->listargs->cmd == LIST_CMD_LSUB) {
        /* special case: for LSUB,
         * mailbox names that match the pattern but aren't subscribed
         * must also be returned if they have a child mailbox that is
         * subscribed */
        perform_output(extname, data->mbentry, rock);
        rock->last_attributes |= MBOX_ATTRIBUTE_CHILDINFO_SUBSCRIBED;
    }

    return 0;
}
