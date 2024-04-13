static void cmd_myrights(const char *tag, const char *name)
{
    mbentry_t *mbentry = NULL;
    int r;
    char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);

    r = mlookup(tag, name, intname, &mbentry);
    if (r == IMAP_MAILBOX_MOVED) return;

    if (!r) r = printmyrights(name, mbentry);

    mboxlist_entry_free(&mbentry);

    imapd_check(NULL, 0);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        return;
    }

    prot_printf(imapd_out, "%s OK %s\r\n", tag,
                error_message(IMAP_OK_COMPLETED));
    free(intname);
}
