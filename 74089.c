static void specialuse_flags(const mbentry_t *mbentry, struct buf *attrib,
                             int isxlist)
{
    if (!mbentry) return;

    char *inbox = mboxname_user_mbox(imapd_userid, NULL);
    int inboxlen = strlen(inbox);

    /* doesn't match inbox, not xlistable */
    if (strncmp(mbentry->name, inbox, inboxlen)) {
        free(inbox);
        return;
    }

    /* inbox - only print if command is XLIST */
    if (mbentry->name[inboxlen] == '\0') {
        if (isxlist) buf_init_ro_cstr(attrib, "\\Inbox");
    }
    /* subdir */
    else if (mbentry->name[inboxlen] == '.') {
        /* check if there's a special use flag set */
        annotatemore_lookup(mbentry->name, "/specialuse", imapd_userid, attrib);
    }
    free(inbox);
    /* otherwise it's actually another user who matches for
     * the substr.  Ok to just print nothing */
}
