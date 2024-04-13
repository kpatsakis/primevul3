static int perform_output(const char *extname, const mbentry_t *mbentry, struct list_rock *rock)
{
    /* skip non-responsive mailboxes early, so they don't break sub folder detection */
    if (mbentry && !imapd_userisadmin) {
        if (mbentry->mbtype == MBTYPE_NETNEWS) return 0;
        if (!(rock->listargs->sel & LIST_SEL_DAV)) {
            if (mboxname_iscalendarmailbox(mbentry->name, mbentry->mbtype)) return 0;
            if (mboxname_isaddressbookmailbox(mbentry->name, mbentry->mbtype)) return 0;
            if (mboxname_isdavdrivemailbox(mbentry->name, mbentry->mbtype)) return 0;
            if (mboxname_isdavnotificationsmailbox(mbentry->name, mbentry->mbtype)) return 0;
        }
    }

    if (mbentry && (mbentry->mbtype & MBTYPE_REMOTE)) {
        struct listargs *listargs = rock->listargs;

        if (hash_lookup(mbentry->server, &rock->server_table)) {
            /* already proxied to this backend server */
            return 0;
        }
        if (listargs->scan ||
            (listargs->ret &
             (LIST_RET_SPECIALUSE | LIST_RET_STATUS | LIST_RET_METADATA))) {
            /* remote mailbox that we need to fetch metadata from */
            struct backend *s;

            hash_insert(mbentry->server,
                        (void *)0xDEADBEEF, &rock->server_table);
            s = proxy_findserver(mbentry->server, &imap_protocol,
                                 proxy_userid, &backend_cached,
                                 &backend_current, &backend_inbox, imapd_in);
            if (s) {
                char mytag[128];

                proxy_gentag(mytag, sizeof(mytag));

                if (listargs->scan) {
                    /* Send SCAN command to backend */
                    prot_printf(s->out, "%s Scan {%tu+}\r\n%s {%tu+}\r\n%s"
                                " {%tu+}\r\n%s\r\n",
                                mytag, strlen(listargs->ref), listargs->ref,
                                strlen(listargs->pat.data[0]),
                                listargs->pat.data[0],
                                strlen(listargs->scan), listargs->scan);

                    pipe_until_tag(s, mytag, 0);
                }
                else {
                    /* Send LIST command to backend */
                    list_data_remote(s, mytag, listargs, rock->subs);
                }
            }

            return 0;
        }
    }

    if (rock->last_name) {
        if (extname) {
            /* same again */
            if (!strcmp(rock->last_name, extname)) return 0;
            size_t extlen = strlen(extname);
            if (extlen < strlen(rock->last_name)
             && rock->last_name[extlen] == imapd_namespace.hier_sep
             && !strncmp(rock->last_name, extname, extlen))
                return 0; /* skip duplicate or reversed calls */
        }
        _addsubs(rock);
        /* check if we need to filter out this mailbox */
        if (!(rock->listargs->sel & LIST_SEL_SUBSCRIBED) ||
            (rock->last_attributes &
             (MBOX_ATTRIBUTE_SUBSCRIBED | MBOX_ATTRIBUTE_CHILDINFO_SUBSCRIBED))) {
            list_response(rock->last_name, rock->last_mbentry,
                          rock->last_attributes, rock->listargs);
        }
        free(rock->last_name);
        rock->last_name = NULL;
        mboxlist_entry_free(&rock->last_mbentry);
    }

    if (extname) {
        rock->last_name = xstrdup(extname);
        if (mbentry) rock->last_mbentry = mboxlist_entry_copy(mbentry);
    }

    rock->last_attributes = 0;
    rock->last_category = 0;

    return 1;
}
