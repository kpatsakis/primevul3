static void cmd_xconvmultisort(char *tag)
{
    int c;
    struct sortcrit *sortcrit = NULL;
    struct searchargs *searchargs = NULL;
    struct windowargs *windowargs = NULL;
    struct conversations_state *cstate = NULL;
    clock_t start = clock();
    char mytime[100];
    int r;

    if (backend_current) {
        /* remote mailbox */
        const char *cmd = "Xconvmultisort";

        prot_printf(backend_current->out, "%s %s ", tag, cmd);
        if (!pipe_command(backend_current, 65536)) {
            pipe_including_tag(backend_current, tag, 0);
        }
        return;
    }
    assert(imapd_index);

    if (!config_getswitch(IMAPOPT_CONVERSATIONS)) {
        prot_printf(imapd_out, "%s BAD Unrecognized command\r\n", tag);
        eatline(imapd_in, ' ');
        return;
    }

    c = getsortcriteria(tag, &sortcrit);
    if (c == EOF) goto error;

    if (c != ' ') {
        prot_printf(imapd_out, "%s BAD Missing window args in XConvMultiSort\r\n",
                    tag);
        goto error;
    }

    c = parse_windowargs(tag, &windowargs, /*updates*/0);
    if (c != ' ')
        goto error;

    /* open the conversations state first - we don't care if it fails,
     * because that probably just means it's already open */
    conversations_open_mbox(index_mboxname(imapd_index), &cstate);

    /* need index loaded to even parse searchargs! */
    searchargs = new_searchargs(tag, GETSEARCH_CHARSET_FIRST,
                                &imapd_namespace, imapd_userid, imapd_authstate,
                                imapd_userisadmin || imapd_userisproxyadmin);
    c = get_search_program(imapd_in, imapd_out, searchargs);
    if (c == EOF) goto error;

    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out,
                    "%s BAD Unexpected extra arguments to XconvMultiSort\r\n", tag);
        goto error;
    }

    r = index_convmultisort(imapd_index, sortcrit, searchargs, windowargs);

    if (r < 0) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag,
                    error_message(r));
        goto error;
    }

    snprintf(mytime, sizeof(mytime), "%2.3f",
             (clock() - start) / (double) CLOCKS_PER_SEC);
    if (CONFIG_TIMING_VERBOSE) {
        char *s = sortcrit_as_string(sortcrit);
        syslog(LOG_DEBUG, "XCONVMULTISORT (%s) processing time %s sec",
               s, mytime);
        free(s);
    }
    prot_printf(imapd_out, "%s OK %s (in %s secs)\r\n", tag,
                error_message(IMAP_OK_COMPLETED), mytime);

out:
    if (cstate) conversations_commit(&cstate);
    freesortcrit(sortcrit);
    freesearchargs(searchargs);
    free_windowargs(windowargs);
    return;

error:
    if (cstate) conversations_commit(&cstate);
    eatline(imapd_in, (c == EOF ? ' ' : c));
    goto out;
}
