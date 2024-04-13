static void cmd_setmetadata(const char *tag, char *mboxpat)
{
    int c, r = 0;
    struct entryattlist *entryatts = NULL;
    annotate_state_t *astate = NULL;

    c = parse_metadata_store_data(tag, &entryatts);
    if (c == EOF) {
        eatline(imapd_in, c);
        goto freeargs;
    }

    /* check for CRLF */
    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out,
                    "%s BAD Unexpected extra arguments to Setmetadata\r\n",
                    tag);
        eatline(imapd_in, c);
        goto freeargs;
    }

    astate = annotate_state_new();
    annotate_state_set_auth(astate, imapd_userisadmin,
                            imapd_userid, imapd_authstate);
    if (!r) {
        if (!*mboxpat) {
            r = annotate_state_set_server(astate);
            if (!r)
                r = annotate_state_store(astate, entryatts);
        }
        else {
            struct annot_store_rock arock;
            arock.entryatts = entryatts;
            r = apply_mailbox_pattern(astate, mboxpat, annot_store_cb, &arock);
        }
    }
    if (!r)
        r = annotate_state_commit(&astate);
    else
        annotate_state_abort(&astate);

    imapd_check(NULL, 0);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
    } else {
        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
    }

  freeargs:
    if (entryatts) freeentryatts(entryatts);
    return;
}
