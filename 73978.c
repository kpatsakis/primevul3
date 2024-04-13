static void cmd_dump(char *tag, char *name, int uid_start)
{
    int r = 0;
    struct mailbox *mailbox = NULL;

    /* administrators only please */
    if (!imapd_userisadmin)
        r = IMAP_PERMISSION_DENIED;

    char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);

    if (!r) r = mailbox_open_irl(intname, &mailbox);

    if (!r) r = dump_mailbox(tag, mailbox, uid_start, MAILBOX_MINOR_VERSION,
                             imapd_in, imapd_out, imapd_authstate);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
    } else {
        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
    }

    if (mailbox) mailbox_close(&mailbox);
    free(intname);
}
