static int checkmboxname(const mbentry_t *mbentry, void *rock)
{
    struct renrock *text = (struct renrock *)rock;
    int r;

    text->found++;

    if((text->nl + strlen(mbentry->name + text->ol)) >= MAX_MAILBOX_BUFFER)
        return IMAP_MAILBOX_BADNAME;

    strcpy(text->newmailboxname + text->nl, mbentry->name + text->ol);

    /* force create, but don't ignore policy.  This is a filthy hack that
       will go away when we refactor this code */
    r = mboxlist_createmailboxcheck(text->newmailboxname, 0, text->partition, 1,
                                    imapd_userid, imapd_authstate, NULL, NULL, 2);
    return r;
}
