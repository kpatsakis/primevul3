void cmd_xconvsort(char *tag, int updates)
{
    int c;
    struct sortcrit *sortcrit = NULL;
    struct searchargs *searchargs = NULL;
    struct windowargs *windowargs = NULL;
    struct index_init init;
    struct index_state *oldstate = NULL;
    struct conversations_state *cstate = NULL;
    clock_t start = clock();
    char mytime[100];
    int r;

    if (backend_current) {
        /* remote mailbox */
        const char *cmd = "Xconvsort";

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
        prot_printf(imapd_out, "%s BAD Missing window args in XConvSort\r\n",
                    tag);
        goto error;
    }

    c = parse_windowargs(tag, &windowargs, updates);
    if (c != ' ')
        goto error;

    /* open the conversations state first - we don't care if it fails,
     * because that probably just means it's already open */
    conversations_open_mbox(index_mboxname(imapd_index), &cstate);

    if (updates) {
        /* in XCONVUPDATES, need to force a re-read from scratch into
         * a new index, because we ask for deleted messages */

        oldstate = imapd_index;
        imapd_index = NULL;

        memset(&init, 0, sizeof(struct index_init));
        init.userid = imapd_userid;
        init.authstate = imapd_authstate;
        init.out = imapd_out;
        init.want_expunged = 1;

        r = index_open(index_mboxname(oldstate), &init, &imapd_index);
        if (r) {
            prot_printf(imapd_out, "%s NO %s\r\n", tag,
                        error_message(r));
            goto error;
        }

        index_checkflags(imapd_index, 0, 0);
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
                    "%s BAD Unexpected extra arguments to Xconvsort\r\n", tag);
        goto error;
    }

    if (updates)
        r = index_convupdates(imapd_index, sortcrit, searchargs, windowargs);
    else
        r = index_convsort(imapd_index, sortcrit, searchargs, windowargs);

    if (oldstate) {
        index_close(&imapd_index);
        imapd_index = oldstate;
    }

    if (r < 0) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag,
                    error_message(r));
        goto error;
    }

    snprintf(mytime, sizeof(mytime), "%2.3f",
             (clock() - start) / (double) CLOCKS_PER_SEC);
    if (CONFIG_TIMING_VERBOSE) {
        char *s = sortcrit_as_string(sortcrit);
        syslog(LOG_DEBUG, "XCONVSORT (%s) processing time %s sec",
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
    if (oldstate) {
        if (imapd_index) index_close(&imapd_index);
        imapd_index = oldstate;
    }
    eatline(imapd_in, (c == EOF ? ' ' : c));
    goto out;
}
