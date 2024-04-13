static void cmd_fetch(char *tag, char *sequence, int usinguid)
{
    const char *cmd = usinguid ? "UID Fetch" : "Fetch";
    struct fetchargs fetchargs;
    int fetchedsomething, r;
    clock_t start = clock();
    char mytime[100];

    if (backend_current) {
        /* remote mailbox */
        prot_printf(backend_current->out, "%s %s %s ", tag, cmd, sequence);
        if (!pipe_command(backend_current, 65536)) {
            pipe_including_tag(backend_current, tag, 0);
        }
        return;
    }

    /* local mailbox */
    memset(&fetchargs, 0, sizeof(struct fetchargs));

    r = parse_fetch_args(tag, cmd,
                         (usinguid && (client_capa & CAPA_QRESYNC)),
                         &fetchargs);
    if (r)
        goto freeargs;

    if (usinguid)
        fetchargs.fetchitems |= FETCH_UID;

    r = index_fetch(imapd_index, sequence, usinguid, &fetchargs,
                &fetchedsomething);

    snprintf(mytime, sizeof(mytime), "%2.3f",
             (clock() - start) / (double) CLOCKS_PER_SEC);

    if (r) {
        prot_printf(imapd_out, "%s NO %s (%s sec)\r\n", tag,
                    error_message(r), mytime);
    } else if (fetchedsomething || usinguid) {
        prot_printf(imapd_out, "%s OK %s (%s sec)\r\n", tag,
                    error_message(IMAP_OK_COMPLETED), mytime);
    } else {
        /* normal FETCH, nothing came back */
        prot_printf(imapd_out, "%s NO %s (%s sec)\r\n", tag,
                    error_message(IMAP_NO_NOSUCHMSG), mytime);
    }

 freeargs:
    fetchargs_fini(&fetchargs);
}
