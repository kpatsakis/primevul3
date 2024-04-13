static void cmd_listrights(char *tag, char *name, char *identifier)
{
    int r, rights;
    mbentry_t *mbentry = NULL;
    struct auth_state *authstate;
    const char *canon_identifier;
    int implicit;
    char rightsdesc[100], optional[33];

    char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);
    r = mlookup(tag, name, intname, &mbentry);
    if (r == IMAP_MAILBOX_MOVED) return;

    if (!r) {
        rights = cyrus_acl_myrights(imapd_authstate, mbentry->acl);

        if (!rights && !imapd_userisadmin &&
            !mboxname_userownsmailbox(imapd_userid, intname)) {
            r = IMAP_MAILBOX_NONEXISTENT;
        }
    }

    mboxlist_entry_free(&mbentry);

    imapd_check(NULL, 0);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        return;
    }

    authstate = auth_newstate(identifier);
    if (global_authisa(authstate, IMAPOPT_ADMINS))
        canon_identifier = identifier; /* don't canonify global admins */
    else
        canon_identifier = canonify_userid(identifier, imapd_userid, NULL);
    auth_freestate(authstate);

    if (!canon_identifier) {
        implicit = 0;
    }
    else if (mboxname_userownsmailbox(canon_identifier, intname)) {
        /* identifier's personal mailbox */
        implicit = config_implicitrights;
    }
    else if (mboxname_isusermailbox(intname, 1)) {
        /* anyone can post to an INBOX */
        implicit = ACL_POST;
    }
    else {
        implicit = 0;
    }

    /* calculate optional rights */
    cyrus_acl_masktostr(implicit ^ (canon_identifier ? ACL_FULL : 0),
                        optional);

    /* build the rights string */
    if (implicit) {
        cyrus_acl_masktostr(implicit, rightsdesc);
    }
    else {
        strcpy(rightsdesc, "\"\"");
    }

    if (*optional) {
        int i, n = strlen(optional);
        char *p = rightsdesc + strlen(rightsdesc);

        for (i = 0; i < n; i++) {
            *p++ = ' ';
            *p++ = optional[i];
        }
        *p = '\0';
    }

    prot_printf(imapd_out, "* LISTRIGHTS ");
    prot_printastring(imapd_out, name);
    (void)prot_putc(' ', imapd_out);
    prot_printastring(imapd_out, identifier);
    prot_printf(imapd_out, " %s", rightsdesc);

    prot_printf(imapd_out, "\r\n%s OK %s\r\n", tag,
                error_message(IMAP_OK_COMPLETED));
    free(intname);
}
