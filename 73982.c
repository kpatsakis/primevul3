static void cmd_getannotation(const char *tag, char *mboxpat)
{
    int c, r = 0;
    strarray_t entries = STRARRAY_INITIALIZER;
    strarray_t attribs = STRARRAY_INITIALIZER;
    annotate_state_t *astate = NULL;

    c = parse_annotate_fetch_data(tag, /*permessage_flag*/0, &entries, &attribs);
    if (c == EOF) {
        eatline(imapd_in, c);
        goto freeargs;
    }

    /* check for CRLF */
    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out,
                    "%s BAD Unexpected extra arguments to Getannotation\r\n",
                    tag);
        eatline(imapd_in, c);
        goto freeargs;
    }

    astate = annotate_state_new();
    annotate_state_set_auth(astate,
                            imapd_userisadmin || imapd_userisproxyadmin,
                            imapd_userid, imapd_authstate);
    if (!*mboxpat) {
        r = annotate_state_set_server(astate);
        if (!r)
            r = annotate_state_fetch(astate, &entries, &attribs,
                                     getannotation_response, NULL);
    }
    else {
        struct annot_fetch_rock arock;
        arock.entries = &entries;
        arock.attribs = &attribs;
        arock.callback = getannotation_response;
        arock.cbrock = NULL;
        r = apply_mailbox_pattern(astate, mboxpat, annot_fetch_cb, &arock);
    }
    /* we didn't write anything */
    annotate_state_abort(&astate);

    imapd_check(NULL, 0);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
    } else {
        prot_printf(imapd_out, "%s OK %s\r\n",
                    tag, error_message(IMAP_OK_COMPLETED));
    }

 freeargs:
    strarray_fini(&entries);
    strarray_fini(&attribs);
}
