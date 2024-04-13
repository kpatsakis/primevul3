static void cmd_xrunannotator(const char *tag, const char *sequence,
                              int usinguid)
{
    const char *cmd = usinguid ? "UID Xrunannotator" : "Xrunannotator";
    clock_t start = clock();
    char mytime[100];
    int c, r = 0;

    if (backend_current) {
        /* remote mailbox */
        prot_printf(backend_current->out, "%s %s %s ", tag, cmd, sequence);
        if (!pipe_command(backend_current, 65536)) {
            pipe_including_tag(backend_current, tag, 0);
        }
        return;
    }

    /* local mailbox */

    /* we're expecting no more arguments */
    c = prot_getc(imapd_in);
    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out, "%s BAD Unexpected extra arguments to %s\r\n", tag, cmd);
        eatline(imapd_in, c);
        return;
    }

    r = index_run_annotator(imapd_index, sequence, usinguid,
                            &imapd_namespace, imapd_userisadmin);

    snprintf(mytime, sizeof(mytime), "%2.3f",
             (clock() - start) / (double) CLOCKS_PER_SEC);

    if (r)
        prot_printf(imapd_out, "%s NO %s (%s sec)\r\n", tag,
                    error_message(r), mytime);
    else
        prot_printf(imapd_out, "%s OK %s (%s sec)\r\n", tag,
                    error_message(IMAP_OK_COMPLETED), mytime);
}
