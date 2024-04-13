static int printmyrights(const char *extname, const mbentry_t *mbentry)
{
    int rights = 0;
    char str[ACL_MAXSTR];

    rights = cyrus_acl_myrights(imapd_authstate, mbentry->acl);

    /* Add in implicit rights */
    if (imapd_userisadmin) {
        rights |= ACL_LOOKUP|ACL_ADMIN;
    }
    else if (mboxname_userownsmailbox(imapd_userid, mbentry->name)) {
        rights |= config_implicitrights;
    }

    if (!(rights & (ACL_LOOKUP|ACL_READ|ACL_INSERT|ACL_CREATE|ACL_DELETEMBOX|ACL_ADMIN))) {
        return IMAP_MAILBOX_NONEXISTENT;
    }

    prot_printf(imapd_out, "* MYRIGHTS ");
    prot_printastring(imapd_out, extname);
    prot_printf(imapd_out, " ");
    prot_printastring(imapd_out, cyrus_acl_masktostr(rights, str));
    prot_printf(imapd_out, "\r\n");

    return 0;
}
