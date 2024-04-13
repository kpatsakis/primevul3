static int list_cb(struct findall_data *data, void *rockp)
{
    struct list_rock *rock = (struct list_rock *)rockp;
    if (!data) {
        if (!(rock->last_attributes & MBOX_ATTRIBUTE_HASCHILDREN))
            rock->last_attributes |= MBOX_ATTRIBUTE_HASNOCHILDREN;
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

    /* list_response will calculate haschildren/hasnochildren flags later
     * if they're required but not yet set, but it's a little cheaper to
     * precalculate them now while we're iterating the mailboxes anyway.
     */
    if (last_name_is_ancestor || (rock->last_name && !data->mbname && !strcmp(rock->last_name, extname)))
        rock->last_attributes |= MBOX_ATTRIBUTE_HASCHILDREN;

    else if (!(rock->last_attributes & MBOX_ATTRIBUTE_HASCHILDREN))
        rock->last_attributes |= MBOX_ATTRIBUTE_HASNOCHILDREN;

    if (!perform_output(data->extname, data->mbentry, rock))
        return 0;

    if (!data->mbname)
        rock->last_attributes |= MBOX_ATTRIBUTE_HASCHILDREN | MBOX_ATTRIBUTE_NONEXISTENT;

    else if (data->mb_category == MBNAME_ALTINBOX)
        rock->last_attributes |= MBOX_ATTRIBUTE_NOINFERIORS;

    return 0;
}
