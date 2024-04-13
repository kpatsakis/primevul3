static void cmd_xstats(char *tag, int c)
{
    int metric;

    if (backend_current) {
        /* remote mailbox */
        const char *cmd = "Xstats";

        prot_printf(backend_current->out, "%s %s ", tag, cmd);
        if (!pipe_command(backend_current, 65536)) {
            pipe_including_tag(backend_current, tag, 0);
        }
        return;
    }

    if (c == EOF) {
        prot_printf(imapd_out, "%s BAD Syntax error in Xstats arguments\r\n", tag);
        goto error;
    }
    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out,
                    "%s BAD Unexpected extra arguments to Xstats\r\n", tag);
        goto error;
    }

    prot_printf(imapd_out, "* XSTATS");
    for (metric = 0 ; metric < XSTATS_NUM_METRICS ; metric++)
        prot_printf(imapd_out, " %s %u", xstats_names[metric], xstats[metric]);
    prot_printf(imapd_out, "\r\n");

    prot_printf(imapd_out, "%s OK %s\r\n", tag,
                error_message(IMAP_OK_COMPLETED));
    return;

error:
    eatline(imapd_in, (c == EOF ? ' ' : c));
}
