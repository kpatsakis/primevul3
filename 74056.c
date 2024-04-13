static int imapd_statusdata(const char *mailboxname, unsigned statusitems,
                            struct statusdata *sd)
{
    int r;
    struct conversations_state *state = NULL;

    if (!(statusitems & STATUS_CONVITEMS)) goto nonconv;
    statusitems &= ~STATUS_CONVITEMS; /* strip them for the regular lookup */

    /* use the existing state if possible */
    state = conversations_get_mbox(mailboxname);

    /* otherwise fetch a new one! */
    if (!state) {
        if (global_conversations) {
            conversations_abort(&global_conversations);
            global_conversations = NULL;
        }
        r = conversations_open_mbox(mailboxname, &state);
        if (r) {
            /* maybe the mailbox doesn't even have conversations - just ignore */
            goto nonconv;
        }
        global_conversations = state;
    }

    r = conversation_getstatus(state, mailboxname, &sd->xconv);
    if (r) return r;

nonconv:
    /* use the index status if we can so we get the 'alive' Recent count */
    if (!strcmpsafe(mailboxname, index_mboxname(imapd_index)) && imapd_index->mailbox)
        return index_status(imapd_index, sd);

    /* fall back to generic lookup */
    return status_lookup(mailboxname, imapd_userid, statusitems, sd);
}
