static void cmd_getacl(const char *tag, const char *name)
{
    int r, access;
    char *acl;
    char *rights, *nextid;
    char *freeme = NULL;
    mbentry_t *mbentry = NULL;

    char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);

    r = mlookup(tag, name, intname, &mbentry);
    if (r == IMAP_MAILBOX_MOVED) return;

    if (!r) {
        access = cyrus_acl_myrights(imapd_authstate, mbentry->acl);

        if (!(access & ACL_ADMIN) &&
            !imapd_userisadmin &&
            !mboxname_userownsmailbox(imapd_userid, intname)) {
            r = (access & ACL_LOOKUP) ?
              IMAP_PERMISSION_DENIED : IMAP_MAILBOX_NONEXISTENT;
        }
    }

    imapd_check(NULL, 0);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        mboxlist_entry_free(&mbentry);
        free(intname);
        return;
    }

    prot_printf(imapd_out, "* ACL ");
    prot_printastring(imapd_out, name);

    freeme = acl = xstrdupnull(mbentry->acl);

    while (acl) {
        rights = strchr(acl, '\t');
        if (!rights) break;
        *rights++ = '\0';

        nextid = strchr(rights, '\t');
        if (!nextid) break;
        *nextid++ = '\0';

        prot_printf(imapd_out, " ");
        prot_printastring(imapd_out, acl);
        prot_printf(imapd_out, " ");
        prot_printastring(imapd_out, rights);
        acl = nextid;
    }
    prot_printf(imapd_out, "\r\n");
    prot_printf(imapd_out, "%s OK %s\r\n", tag,
                error_message(IMAP_OK_COMPLETED));
    free(freeme);
    mboxlist_entry_free(&mbentry);
    free(intname);
}
