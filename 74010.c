static void cmd_thread(char *tag, int usinguid)
{
    static struct buf arg;
    int c;
    int alg;
    struct searchargs *searchargs;
    clock_t start = clock();
    char mytime[100];
    int n;

    if (backend_current) {
        /* remote mailbox */
        const char *cmd = usinguid ? "UID Thread" : "Thread";

        prot_printf(backend_current->out, "%s %s ", tag, cmd);
        if (!pipe_command(backend_current, 65536)) {
            pipe_including_tag(backend_current, tag, 0);
        }
        return;
    }

    /* local mailbox */
    /* get algorithm */
    c = getword(imapd_in, &arg);
    if (c != ' ') {
        prot_printf(imapd_out, "%s BAD Missing algorithm in Thread\r\n", tag);
        eatline(imapd_in, c);
        return;
    }

    if ((alg = find_thread_algorithm(arg.s)) == -1) {
        prot_printf(imapd_out, "%s BAD Invalid Thread algorithm %s\r\n",
                    tag, arg.s);
        eatline(imapd_in, c);
        return;
    }

    searchargs = new_searchargs(tag, GETSEARCH_CHARSET_FIRST,
                                &imapd_namespace, imapd_userid, imapd_authstate,
                                imapd_userisadmin || imapd_userisproxyadmin);
    c = get_search_program(imapd_in, imapd_out, searchargs);
    if (c == EOF) {
        eatline(imapd_in, ' ');
        freesearchargs(searchargs);
        return;
    }

    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out,
                    "%s BAD Unexpected extra arguments to Thread\r\n", tag);
        eatline(imapd_in, c);
        freesearchargs(searchargs);
        return;
    }

    n = index_thread(imapd_index, alg, searchargs, usinguid);
    snprintf(mytime, sizeof(mytime), "%2.3f",
             (clock() - start) / (double) CLOCKS_PER_SEC);
    prot_printf(imapd_out, "%s OK %s (%d msgs in %s secs)\r\n", tag,
                error_message(IMAP_OK_COMPLETED), n, mytime);

    freesearchargs(searchargs);
    return;
}
