static int apply_mailbox_pattern(annotate_state_t *state,
                                 const char *pattern,
                                 int (*proc)(annotate_state_t *, void *),
                                 void *data)
{
    struct apply_rock arock;
    int r = 0;

    memset(&arock, 0, sizeof(arock));
    arock.state = state;
    arock.proc = proc;
    arock.data = data;

    r = mboxlist_findall(&imapd_namespace,
                         pattern,
                         imapd_userisadmin || imapd_userisproxyadmin,
                         imapd_userid,
                         imapd_authstate,
                         apply_cb, &arock);

    if (!r && !arock.nseen)
        r = IMAP_MAILBOX_NONEXISTENT;

    return r;
}
