static void cmd_search(char *tag, int usinguid)
{
    int c;
    struct searchargs *searchargs;
    clock_t start = clock();
    char mytime[100];
    int n;

    if (backend_current) {
        /* remote mailbox */
        const char *cmd = usinguid ? "UID Search" : "Search";

        prot_printf(backend_current->out, "%s %s ", tag, cmd);
        if (!pipe_command(backend_current, 65536)) {
            pipe_including_tag(backend_current, tag, 0);
        }
        return;
    }

    /* local mailbox */
    searchargs = new_searchargs(tag, GETSEARCH_CHARSET_KEYWORD|GETSEARCH_RETURN,
                                &imapd_namespace, imapd_userid, imapd_authstate,
                                imapd_userisadmin || imapd_userisproxyadmin);

    /* special case quirk for iPhones */
    if (imapd_id.quirks & QUIRK_SEARCHFUZZY)
        searchargs->fuzzy_depth++;

    c = get_search_program(imapd_in, imapd_out, searchargs);
    if (c == EOF) {
        eatline(imapd_in, ' ');
        freesearchargs(searchargs);
        return;
    }

    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out, "%s BAD Unexpected extra arguments to Search\r\n", tag);
        eatline(imapd_in, c);
        freesearchargs(searchargs);
        return;
    }

    if (searchargs->charset == CHARSET_UNKNOWN_CHARSET) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag,
               error_message(IMAP_UNRECOGNIZED_CHARSET));
    }
    else {
        n = index_search(imapd_index, searchargs, usinguid);
        snprintf(mytime, sizeof(mytime), "%2.3f",
                 (clock() - start) / (double) CLOCKS_PER_SEC);
        prot_printf(imapd_out, "%s OK %s (%d msgs in %s secs)\r\n", tag,
                    error_message(IMAP_OK_COMPLETED), n, mytime);
    }

    freesearchargs(searchargs);
}
