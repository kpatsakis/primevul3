void cmd_xbackup(const char *tag,
                 const char *mailbox,
                 const char *channel)
{
    ptrarray_t list = PTRARRAY_INITIALIZER;
    int i, r;

    /* admins only please */
    if (!imapd_userisadmin && !imapd_userisproxyadmin) {
        r = IMAP_PERMISSION_DENIED;
        goto done;
    }

    if (!config_getswitch(IMAPOPT_XBACKUP_ENABLED)) {
        /* shouldn't get here, but just in case */
        r = IMAP_PERMISSION_DENIED;
        goto done;
    }

    mboxlist_findall(NULL, mailbox, 1, NULL, NULL, xbackup_addmbox, &list);

    if (list.count) {
        r = do_xbackup(channel, &list);

        for (i = 0; i < list.count; i++) {
            mbname_t *mbname = ptrarray_nth(&list, i);
            if (mbname)
                mbname_free(&mbname);
        }
        ptrarray_fini(&list);
    }
    else {
        r = IMAP_MAILBOX_NONEXISTENT;
    }

done:
    imapd_check(NULL, 0);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
    } else {
        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
    }
}
