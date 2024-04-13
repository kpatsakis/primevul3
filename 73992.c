static void cmd_noop(char *tag, char *cmd)
{
    if (backend_current) {
        /* remote mailbox */
        prot_printf(backend_current->out, "%s %s\r\n", tag, cmd);
        pipe_including_tag(backend_current, tag, 0);

        return;
    }

    index_check(imapd_index, 1, 0);

    prot_printf(imapd_out, "%s OK %s\r\n", tag,
                error_message(IMAP_OK_COMPLETED));
}
