static int xconvfetch_lookup(struct conversations_state *statep,
                             conversation_id_t cid,
                             modseq_t ifchangedsince,
                             hash_table *wanted_cids,
                             strarray_t *folder_list)
{
    const char *key = conversation_id_encode(cid);
    conversation_t *conv = NULL;
    conv_folder_t *folder;
    int r;

    r = conversation_load(statep, cid, &conv);
    if (r) return r;

    if (!conv)
        goto out;

    if (!conv->exists)
        goto out;

    /* output the metadata for this conversation */
    {
        struct dlist *dl = dlist_newlist(NULL, "");
        dlist_setatom(dl, "", "MODSEQ");
        do_one_xconvmeta(statep, cid, conv, dl);
        dlist_free(&dl);
    }

    if (ifchangedsince >= conv->modseq)
        goto out;

    hash_insert(key, (void *)1, wanted_cids);

    for (folder = conv->folders; folder; folder = folder->next) {
        /* no contents */
        if (!folder->exists)
            continue;

        /* finally, something worth looking at */
        strarray_add(folder_list, strarray_nth(statep->folder_names, folder->number));
    }

out:
    conversation_free(conv);
    return 0;
}
