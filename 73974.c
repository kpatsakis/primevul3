static void cmd_close(char *tag, char *cmd)
{
    if (backend_current) {
        /* remote mailbox */
        prot_printf(backend_current->out, "%s %s\r\n", tag, cmd);
        /* xxx do we want this to say OK if the connection is gone?
         * saying NO is clearly wrong, hense the fatal request. */
        pipe_including_tag(backend_current, tag, 0);

        /* remove backend_current from the protgroup */
        protgroup_delete(protin, backend_current->in);

        backend_current = NULL;
        return;
    }

    /* local mailbox */
    if ((cmd[0] == 'C') && index_hasrights(imapd_index, ACL_EXPUNGE)) {
        index_expunge(imapd_index, NULL, 1);
        /* don't tell changes here */
    }

    index_close(&imapd_index);

    /* http://www.rfc-editor.org/errata_search.php?rfc=5162
     * Errata ID: 1808 - don't send HIGHESTMODSEQ to a close
     * command, because it can lose synchronisation */
    prot_printf(imapd_out, "%s OK %s\r\n",
                tag, error_message(IMAP_OK_COMPLETED));
}
