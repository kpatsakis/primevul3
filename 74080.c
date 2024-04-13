static int recursivematch_cb(struct findall_data *data, void *rockp)
{
    if (!data) return 0;

    struct list_rock_recursivematch *rock = (struct list_rock_recursivematch *)rockp;
    list_callback_calls++;

    const char *extname = data->extname;

    /* skip non-responsive mailboxes early, so they don't break sub folder detection */
    if (!(imapd_userisadmin || (rock->listargs->sel & LIST_SEL_DAV))) {
        mbname_t *mbname = (mbname_t *) data->mbname;
        const char *intname;
        int r;

        if (!mbname) {
            mbname = mbname_from_extname(extname, &imapd_namespace, imapd_userid);
        }

        intname = mbname_intname(mbname);
        r = mboxname_iscalendarmailbox(intname, 0) ||
            mboxname_isaddressbookmailbox(intname, 0) ||
            mboxname_isdavdrivemailbox(intname, 0) ||
            mboxname_isdavnotificationsmailbox(intname, 0);

        if (!data->mbname) mbname_free(&mbname);

        if (r) return 0;
    }

    uint32_t *list_info = hash_lookup(extname, &rock->table);
    if (!list_info) {
        list_info = xzmalloc(sizeof(uint32_t));
        hash_insert(extname, list_info, &rock->table);
        rock->count++;
    }

    if (data->mbname) { /* exact match */
        *list_info |= MBOX_ATTRIBUTE_SUBSCRIBED;
    }
    else {
        *list_info |= MBOX_ATTRIBUTE_CHILDINFO_SUBSCRIBED | MBOX_ATTRIBUTE_HASCHILDREN;
    }

    return 0;
}
