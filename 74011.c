static void cmd_undump(char *tag, char *name)
{
    int r = 0;
    char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);

    /* administrators only please */
    if (!imapd_userisadmin)
        r = IMAP_PERMISSION_DENIED;

    if (!r) r = mlookup(tag, name, intname, NULL);

    if (!r) r = undump_mailbox(intname, imapd_in, imapd_out, imapd_authstate);

    if (r) {
        prot_printf(imapd_out, "%s NO %s%s\r\n",
                    tag,
                    (r == IMAP_MAILBOX_NONEXISTENT &&
                     mboxlist_createmailboxcheck(intname, 0, 0,
                                                 imapd_userisadmin,
                                                 imapd_userid, imapd_authstate,
                                                 NULL, NULL, 0) == 0)
                    ? "[TRYCREATE] " : "", error_message(r));
    } else {
        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
    }
    free(intname);
}
