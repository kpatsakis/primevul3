static void do_one_xconvmeta(struct conversations_state *state,
                             conversation_id_t cid,
                             conversation_t *conv,
                             struct dlist *itemlist)
{
    struct dlist *item = dlist_newpklist(NULL, "");
    struct dlist *fl;

    assert(conv);
    assert(itemlist);

    for (fl = itemlist->head; fl; fl = fl->next) {
        const char *key = dlist_cstring(fl);

        /* xxx - parse to a fetchitems? */
        if (!strcasecmp(key, "MODSEQ"))
            dlist_setnum64(item, "MODSEQ", conv->modseq);
        else if (!strcasecmp(key, "EXISTS"))
            dlist_setnum32(item, "EXISTS", conv->exists);
        else if (!strcasecmp(key, "UNSEEN"))
            dlist_setnum32(item, "UNSEEN", conv->unseen);
        else if (!strcasecmp(key, "SIZE"))
            dlist_setnum32(item, "SIZE", conv->size);
        else if (!strcasecmp(key, "COUNT")) {
            struct dlist *flist = dlist_newlist(item, "COUNT");
            fl = fl->next;
            if (dlist_isatomlist(fl)) {
                struct dlist *tmp;
                for (tmp = fl->head; tmp; tmp = tmp->next) {
                    const char *lookup = dlist_cstring(tmp);
                    int i = strarray_find_case(state->counted_flags, lookup, 0);
                    if (i >= 0) {
                        dlist_setflag(flist, "FLAG", lookup);
                        dlist_setnum32(flist, "COUNT", conv->counts[i]);
                    }
                }
            }
        }
        else if (!strcasecmp(key, "SENDERS")) {
            conv_sender_t *sender;
            struct dlist *slist = dlist_newlist(item, "SENDERS");
            for (sender = conv->senders; sender; sender = sender->next) {
                struct dlist *sli = dlist_newlist(slist, "");
                dlist_setatom(sli, "NAME", sender->name);
                dlist_setatom(sli, "ROUTE", sender->route);
                dlist_setatom(sli, "MAILBOX", sender->mailbox);
                dlist_setatom(sli, "DOMAIN", sender->domain);
            }
        }
        /* XXX - maybe rename FOLDERCOUNTS or something? */
        else if (!strcasecmp(key, "FOLDEREXISTS")) {
            struct dlist *flist = dlist_newlist(item, "FOLDEREXISTS");
            conv_folder_t *folder;
            fl = fl->next;
            if (dlist_isatomlist(fl)) {
                struct dlist *tmp;
                for (tmp = fl->head; tmp; tmp = tmp->next) {
                    const char *extname = dlist_cstring(tmp);
                    char *intname = mboxname_from_external(extname, &imapd_namespace, imapd_userid);
                    folder = conversation_find_folder(state, conv, intname);
                    free(intname);
                    dlist_setatom(flist, "MBOXNAME", extname);
                    /* ok if it's not there */
                    dlist_setnum32(flist, "EXISTS", folder ? folder->exists : 0);
                }
            }
        }
        else if (!strcasecmp(key, "FOLDERUNSEEN")) {
            struct dlist *flist = dlist_newlist(item, "FOLDERUNSEEN");
            conv_folder_t *folder;
            fl = fl->next;
            if (dlist_isatomlist(fl)) {
                struct dlist *tmp;
                for (tmp = fl->head; tmp; tmp = tmp->next) {
                    const char *extname = dlist_cstring(tmp);
                    char *intname = mboxname_from_external(extname, &imapd_namespace, imapd_userid);
                    folder = conversation_find_folder(state, conv, intname);
                    free(intname);
                    dlist_setatom(flist, "MBOXNAME", extname);
                    /* ok if it's not there */
                    dlist_setnum32(flist, "UNSEEN", folder ? folder->unseen : 0);
                }
            }
        }
        else {
            dlist_setatom(item, key, NULL); /* add a NIL response */
        }
    }

    prot_printf(imapd_out, "* XCONVMETA %s ", conversation_id_encode(cid));
    dlist_print(item, 0, imapd_out);
    prot_printf(imapd_out, "\r\n");

    dlist_free(&item);
}
