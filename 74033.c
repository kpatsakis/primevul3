static void do_xconvmeta(const char *tag,
                         struct conversations_state *state,
                         struct dlist *cidlist,
                         struct dlist *itemlist)
{
    conversation_id_t cid;
    struct dlist *dl;
    int r;

    for (dl = cidlist->head; dl; dl = dl->next) {
        const char *cidstr = dlist_cstring(dl);
        conversation_t *conv = NULL;

        if (!conversation_id_decode(&cid, cidstr) || !cid) {
            prot_printf(imapd_out, "%s BAD Invalid CID %s\r\n", tag, cidstr);
            return;
        }

        r = conversation_load(state, cid, &conv);
        if (r) {
            prot_printf(imapd_out, "%s BAD Failed to read %s\r\n", tag, cidstr);
            conversation_free(conv);
            return;
        }

        if (conv && conv->exists)
            do_one_xconvmeta(state, cid, conv, itemlist);

        conversation_free(conv);
    }

    prot_printf(imapd_out, "%s OK Completed\r\n", tag);
}
