static int renmbox(const mbentry_t *mbentry, void *rock)
{
    struct renrock *text = (struct renrock *)rock;
    char *oldextname = NULL, *newextname = NULL;
    int r = 0;
    uint32_t uidvalidity = mbentry->uidvalidity;

    if((text->nl + strlen(mbentry->name + text->ol)) >= MAX_MAILBOX_BUFFER)
        goto done;

    strcpy(text->newmailboxname + text->nl, mbentry->name + text->ol);

    /* check if a previous deleted mailbox existed */
    mbentry_t *newmbentry = NULL;
    r = mboxlist_lookup_allow_all(text->newmailboxname, &newmbentry, NULL);
    /* XXX - otherwise we should probably reject now, but meh, save it for
     * a real cleanup */
    if (!r && newmbentry->mbtype == MBTYPE_DELETED) {
        /* changing the unique id since last time? */
        if (strcmpsafe(mbentry->uniqueid, newmbentry->uniqueid)) {
            /* then the UIDVALIDITY must be higher than before */
            if (uidvalidity <= newmbentry->uidvalidity)
                uidvalidity = newmbentry->uidvalidity+1;
        }
    }
    mboxlist_entry_free(&newmbentry);


    /* don't notify implied rename in mailbox hierarchy */
    r = mboxlist_renamemailbox(mbentry->name, text->newmailboxname,
                               text->partition, uidvalidity,
                               1, imapd_userid, imapd_authstate, NULL, 0, 0,
                               text->rename_user);

    oldextname =
        mboxname_to_external(mbentry->name, &imapd_namespace, imapd_userid);
    newextname =
        mboxname_to_external(text->newmailboxname, &imapd_namespace, imapd_userid);

    if(r) {
        prot_printf(imapd_out, "* NO rename %s %s: %s\r\n",
                    oldextname, newextname, error_message(r));
        if (!RENAME_STOP_ON_ERROR) r = 0;
    } else {
        /* If we're renaming a user, change quotaroot and ACL */
        if (text->rename_user) {
            user_copyquotaroot(mbentry->name, text->newmailboxname);
            user_renameacl(text->namespace, text->newmailboxname,
                           text->olduser, text->newuser);
        }


        prot_printf(imapd_out, "* OK rename %s %s\r\n",
                    oldextname, newextname);
    }

done:
    prot_flush(imapd_out);
    free(oldextname);
    free(newextname);

    return r;
}
