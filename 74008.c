static void cmd_syncrestart(const char *tag, struct sync_reserve_list **reserve_listp, int re_alloc)
{
    struct sync_reserve *res;
    struct sync_reserve_list *l = *reserve_listp;
    struct sync_msgid *msg;
    int hash_size = l->hash_size;
    struct partition_list *p, *pl = NULL;

    for (res = l->head; res; res = res->next) {
        for (msg = res->list->head; msg; msg = msg->next) {
            if (!msg->fname) continue;
            pl = partition_list_add(res->part, pl);

            unlink(msg->fname);
        }
    }
    sync_reserve_list_free(reserve_listp);

    /* Remove all <partition>/sync./<pid> directories referred to above */
    for (p=pl; p ; p = p->next) {
        static char buf[MAX_MAILBOX_PATH];

        snprintf(buf, MAX_MAILBOX_PATH, "%s/sync./%lu",
                 config_partitiondir(p->name), (unsigned long)getpid());
        rmdir(buf);

        /* and the archive partition too */
        snprintf(buf, MAX_MAILBOX_PATH, "%s/sync./%lu",
                 config_archivepartitiondir(p->name), (unsigned long)getpid());
        rmdir(buf);
    }
    partition_list_free(pl);

    if (re_alloc) {
        *reserve_listp = sync_reserve_list_create(hash_size);
        prot_printf(imapd_out, "%s OK Restarting\r\n", tag);
    }
    else
        *reserve_listp = NULL;
}
