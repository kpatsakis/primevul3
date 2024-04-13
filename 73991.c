static void cmd_namespace(char* tag)
{
    int sawone[3] = {0, 0, 0};
    char* pattern;

    if (SLEEZY_NAMESPACE) {
        if (strlen(imapd_userid) + 5 >= MAX_MAILBOX_BUFFER)
            sawone[NAMESPACE_INBOX] = 0;
        else {
            char *inbox = mboxname_user_mbox(imapd_userid, NULL);
            sawone[NAMESPACE_INBOX] =
                !mboxlist_lookup(inbox, NULL, NULL);
            free(inbox);
        }
        sawone[NAMESPACE_USER] = imapd_userisadmin ? 1 : imapd_namespace.accessible[NAMESPACE_USER];
        sawone[NAMESPACE_SHARED] = imapd_userisadmin ? 1 : imapd_namespace.accessible[NAMESPACE_SHARED];
    } else {
        pattern = xstrdup("%");
        /* now find all the exciting toplevel namespaces -
         * we're using internal names here
         */
        mboxlist_findall(NULL, pattern, imapd_userisadmin, imapd_userid,
                         imapd_authstate, namespacedata, (void*) sawone);
        free(pattern);
    }

    prot_printf(imapd_out, "* NAMESPACE");
    if (sawone[NAMESPACE_INBOX]) {
        prot_printf(imapd_out, " ((\"%s\" \"%c\"))",
                    imapd_namespace.prefix[NAMESPACE_INBOX],
                    imapd_namespace.hier_sep);
    } else {
        prot_printf(imapd_out, " NIL");
    }
    if (sawone[NAMESPACE_USER]) {
        prot_printf(imapd_out, " ((\"%s\" \"%c\"))",
                    imapd_namespace.prefix[NAMESPACE_USER],
                    imapd_namespace.hier_sep);
    } else {
        prot_printf(imapd_out, " NIL");
    }
    if (sawone[NAMESPACE_SHARED]) {
        prot_printf(imapd_out, " ((\"%s\" \"%c\"))",
                    imapd_namespace.prefix[NAMESPACE_SHARED],
                    imapd_namespace.hier_sep);
    } else {
        prot_printf(imapd_out, " NIL");
    }
    prot_printf(imapd_out, "\r\n");

    imapd_check(NULL, 0);

    prot_printf(imapd_out, "%s OK %s\r\n", tag,
                error_message(IMAP_OK_COMPLETED));
}
