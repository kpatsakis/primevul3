static void cmd_xsnippets(char *tag)
{
    int c;
    struct searchargs *searchargs = NULL;
    struct snippetargs *snippetargs = NULL;
    clock_t start = clock();
    char mytime[100];
    int r;

    if (backend_current) {
        /* remote mailbox */
        const char *cmd = "Xsnippets";

        prot_printf(backend_current->out, "%s %s ", tag, cmd);
        if (!pipe_command(backend_current, 65536)) {
            pipe_including_tag(backend_current, tag, 0);
        }
        return;
    }
    assert(imapd_index);

    c = get_snippetargs(&snippetargs);
    if (c == EOF) {
        prot_printf(imapd_out, "%s BAD Syntax error in snippet arguments\r\n", tag);
        goto error;
    }
    if (c != ' ') {
        prot_printf(imapd_out,
                    "%s BAD Unexpected arguments in Xsnippets\r\n", tag);
        goto error;
    }

    /* need index loaded to even parse searchargs! */
    searchargs = new_searchargs(tag, GETSEARCH_CHARSET_FIRST,
                                &imapd_namespace, imapd_userid, imapd_authstate,
                                imapd_userisadmin || imapd_userisproxyadmin);
    c = get_search_program(imapd_in, imapd_out, searchargs);
    if (c == EOF) goto error;

    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out,
                    "%s BAD Unexpected extra arguments to Xsnippets\r\n", tag);
        goto error;
    }

    r = index_snippets(imapd_index, snippetargs, searchargs);

    if (r < 0) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag,
                    error_message(r));
        goto error;
    }

    snprintf(mytime, sizeof(mytime), "%2.3f",
             (clock() - start) / (double) CLOCKS_PER_SEC);
    prot_printf(imapd_out, "%s OK %s (in %s secs)\r\n", tag,
                error_message(IMAP_OK_COMPLETED), mytime);

out:
    freesearchargs(searchargs);
    free_snippetargs(&snippetargs);
    return;

error:
    eatline(imapd_in, (c == EOF ? ' ' : c));
    goto out;
}
