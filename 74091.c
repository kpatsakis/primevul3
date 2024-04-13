static int sync_mailbox(struct mailbox *mailbox,
                        struct sync_folder_list *replica_folders,
                        const char *topart,
                        struct backend *be)
{
    int r = 0;
    struct sync_folder_list *master_folders = NULL;
    struct sync_reserve_list *reserve_guids = NULL;
    struct sync_msgid_list *part_list;
    struct sync_reserve *reserve;
    struct sync_folder *mfolder, *rfolder;
    struct sync_annot_list *annots = NULL;
    modseq_t xconvmodseq = 0;
    unsigned flags = SYNC_FLAG_LOGGING | SYNC_FLAG_LOCALONLY;

    if (!topart) topart = mailbox->part;
    reserve_guids = sync_reserve_list_create(SYNC_MSGID_LIST_HASH_SIZE);
    part_list = sync_reserve_partlist(reserve_guids, topart);

    /* always send mailbox annotations */
    r = read_annotations(mailbox, NULL, &annots);
    if (r) {
        syslog(LOG_ERR, "sync_mailbox(): read annotations failed: %s '%s'",
               mailbox->name, error_message(r));
        goto cleanup;
    }

    /* xconvmodseq */
    if (mailbox_has_conversations(mailbox)) {
        r = mailbox_get_xconvmodseq(mailbox, &xconvmodseq);
        if (r) {
            syslog(LOG_ERR, "sync_mailbox(): mailbox get xconvmodseq failed: %s '%s'",
                mailbox->name, error_message(r));
            goto cleanup;
        }
    }

    master_folders = sync_folder_list_create();
    sync_folder_list_add(master_folders,
                         mailbox->uniqueid, mailbox->name,
                         mailbox->mbtype,
                         mailbox->part,
                         mailbox->acl,
                         mailbox->i.options,
                         mailbox->i.uidvalidity,
                         mailbox->i.last_uid,
                         mailbox->i.highestmodseq,
                         mailbox->i.synccrcs,
                         mailbox->i.recentuid,
                         mailbox->i.recenttime,
                         mailbox->i.pop3_last_login,
                         mailbox->i.pop3_show_after,
                         annots,
                         xconvmodseq,
                         /* ispartial */0);
    annots = NULL; /* list took ownership */

    mfolder = master_folders->head;
    /* when mfolder->mailbox is set, sync_update_mailbox will use it rather
     * than obtaining its own (short-lived) locks */
    mfolder->mailbox = mailbox;

    uint32_t fromuid = 0;
    rfolder = sync_folder_lookup(replica_folders, mfolder->uniqueid);
    if (rfolder) {
        rfolder->mark = 1;

        /* does it need a rename? */
        if (strcmp(mfolder->name, rfolder->name) ||
            strcmp(topart, rfolder->part)) {
            /* bail and retry */
            syslog(LOG_NOTICE,
                   "XFER: rename %s!%s -> %s!%s during final sync"
                   " - must try XFER again",
                   mfolder->name, mfolder->part, rfolder->name, rfolder->part);
            r = IMAP_AGAIN;
            goto cleanup;
        }
        fromuid = rfolder->last_uid;
    }
    sync_find_reserve_messages(mailbox, fromuid, mailbox->i.last_uid, part_list);

    reserve = reserve_guids->head;
    r = sync_reserve_partition(reserve->part, replica_folders,
                               reserve->list, be);
    if (r) {
        syslog(LOG_ERR, "sync_mailbox(): reserve partition failed: %s '%s'",
               mfolder->name, error_message(r));
        goto cleanup;
    }

    r = sync_update_mailbox(mfolder, rfolder, topart, reserve_guids, be, flags);
    if (r) {
        syslog(LOG_ERR, "sync_mailbox(): update failed: %s '%s'",
                mfolder->name, error_message(r));
    }

  cleanup:
    sync_reserve_list_free(&reserve_guids);
    sync_folder_list_free(&master_folders);
    sync_annot_list_free(&annots);

    return r;
}
