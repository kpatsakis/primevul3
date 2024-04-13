static int do_xconvfetch(struct dlist *cidlist,
                         modseq_t ifchangedsince,
                         struct fetchargs *fetchargs)
{
    struct conversations_state *state = NULL;
    int r = 0;
    struct index_state *index_state = NULL;
    struct dlist *dl;
    hash_table wanted_cids = HASH_TABLE_INITIALIZER;
    strarray_t folder_list = STRARRAY_INITIALIZER;
    struct index_init init;
    int i;

    r = conversations_open_user(imapd_userid, &state);
    if (r) goto out;

    construct_hash_table(&wanted_cids, 1024, 0);

    for (dl = cidlist->head; dl; dl = dl->next) {
        r = xconvfetch_lookup(state, dlist_num(dl), ifchangedsince,
                              &wanted_cids, &folder_list);
        if (r) goto out;
    }

    /* unchanged, woot */
    if (!folder_list.count)
        goto out;

    fetchargs->cidhash = &wanted_cids;

    memset(&init, 0, sizeof(struct index_init));
    init.userid = imapd_userid;
    init.authstate = imapd_authstate;
    init.out = imapd_out;

    for (i = 0; i < folder_list.count; i++) {
        const char *mboxname = folder_list.data[i];

        r = index_open(mboxname, &init, &index_state);
        if (r == IMAP_MAILBOX_NONEXISTENT)
            continue;
        if (r)
            goto out;

        index_checkflags(index_state, 0, 0);

        /* make sure \Deleted messages are expunged.  Will also lock the
         * mailbox state and read any new information */
        r = index_expunge(index_state, NULL, 1);

        if (!r)
            index_fetchresponses(index_state, NULL, /*usinguid*/1,
                                 fetchargs, NULL);

        index_close(&index_state);

        if (r) goto out;
    }

    r = 0;

out:
    index_close(&index_state);
    conversations_commit(&state);
    free_hash_table(&wanted_cids, NULL);
    strarray_fini(&folder_list);
    return r;
}
