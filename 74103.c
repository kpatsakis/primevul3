static int xfer_finalsync(struct xfer_header *xfer)
{
    struct sync_name_list *master_quotaroots = sync_name_list_create();
    struct sync_folder_list *replica_folders = sync_folder_list_create();
    struct sync_folder *rfolder;
    struct sync_name_list *replica_subs = NULL;
    struct sync_sieve_list *replica_sieve = NULL;
    struct sync_seen_list *replica_seen = NULL;
    struct sync_quota_list *replica_quota = sync_quota_list_create();
    const char *cmd;
    struct dlist *kl = NULL;
    struct xfer_item *item;
    struct mailbox *mailbox = NULL;
    mbentry_t newentry;
    unsigned flags = SYNC_FLAG_LOGGING | SYNC_FLAG_LOCALONLY;
    int r;

    if (xfer->userid) {
        syslog(LOG_INFO, "XFER: final sync of user %s", xfer->userid);

        replica_subs = sync_name_list_create();
        replica_sieve = sync_sieve_list_create();
        replica_seen = sync_seen_list_create();

        cmd = "USER";
        kl = dlist_setatom(NULL, cmd, xfer->userid);
    }
    else {
        syslog(LOG_INFO, "XFER: final sync of mailbox %s",
               xfer->items->mbentry->name);

        cmd = "MAILBOXES";
        kl = dlist_newlist(NULL, cmd);
        dlist_setatom(kl, "MBOXNAME", xfer->items->mbentry->name);
    }

    sync_send_lookup(kl, xfer->be->out);
    dlist_free(&kl);

    r = sync_response_parse(xfer->be->in, cmd, replica_folders, replica_subs,
                            replica_sieve, replica_seen, replica_quota);

    if (r) goto done;

    for (item = xfer->items; item; item = item->next) {
        r = mailbox_open_iwl(item->mbentry->name, &mailbox);
        if (!r) r = sync_mailbox_version_check(&mailbox);
        if (r) {
            syslog(LOG_ERR,
                   "Failed to open mailbox %s for xfer_final_sync() %s",
                   item->mbentry->name, error_message(r));
            goto done;
        }

        /* Open cyrus.annotations before we set mailbox to MOVING and
           change its location to destination server and partition */
        r = mailbox_get_annotate_state(mailbox, ANNOTATE_ANY_UID, NULL);
        if (r) {
            syslog(LOG_ERR,
                   "Failed to get annotate state for mailbox %s"
                   " for xfer_final_sync() %s",
                   mailbox->name, error_message(r));
            mailbox_close(&mailbox);
            goto done;
        }

        /* Step 3.5: Set mailbox as MOVING on local server */
        /* XXX - this code is awful... need a sane way to manage mbentries */
        newentry.name = item->mbentry->name;
        newentry.acl = item->mbentry->acl;
        newentry.uniqueid = item->mbentry->uniqueid;
        newentry.uidvalidity = item->mbentry->uidvalidity;
        newentry.mbtype = item->mbentry->mbtype|MBTYPE_MOVING;
        newentry.server = xfer->toserver;
        newentry.partition = xfer->topart;
        r = mboxlist_update(&newentry, 1);

        if (r) {
            syslog(LOG_ERR,
                   "Could not move mailbox: %s, mboxlist_update() failed %s",
                   item->mbentry->name, error_message(r));
        }
        else item->state = XFER_LOCAL_MOVING;

        /* Step 4: Sync local -> remote */
        if (!r) {
            r = sync_mailbox(mailbox, replica_folders, xfer->topart, xfer->be);
            if (r) {
                syslog(LOG_ERR,
                       "Could not move mailbox: %s, sync_mailbox() failed %s",
                       item->mbentry->name, error_message(r));
            }
            else {
                if (mailbox->quotaroot &&
                    !sync_name_lookup(master_quotaroots, mailbox->quotaroot)) {
                    sync_name_list_add(master_quotaroots, mailbox->quotaroot);
                }

                r = sync_do_annotation(mailbox->name, xfer->be, flags);
                if (r) {
                    syslog(LOG_ERR, "Could not move mailbox: %s,"
                           " sync_do_annotation() failed %s",
                           item->mbentry->name, error_message(r));
                }
            }
        }

        mailbox_close(&mailbox);

        if (r) goto done;

        item->state = XFER_UNDUMPED;
    }

    /* Delete folders on replica which no longer exist on master */
    for (rfolder = replica_folders->head; rfolder; rfolder = rfolder->next) {
        if (rfolder->mark) continue;

        r = sync_folder_delete(rfolder->name, xfer->be, flags);
        if (r) {
            syslog(LOG_ERR, "sync_folder_delete(): failed: %s '%s'",
                   rfolder->name, error_message(r));
            goto done;
        }
    }

    /* Handle any mailbox/user metadata */
    r = sync_do_user_quota(master_quotaroots, replica_quota, xfer->be, flags);
    if (!r && xfer->userid) {
        r = sync_do_user_seen(xfer->userid, replica_seen, xfer->be, flags);
        if (!r) r = sync_do_user_sub(xfer->userid, replica_subs,
                                     xfer->be, flags);
        if (!r) r = sync_do_user_sieve(xfer->userid, replica_sieve,
                                       xfer->be, flags);
    }

  done:
    sync_name_list_free(&master_quotaroots);
    sync_folder_list_free(&replica_folders);
    if (replica_subs) sync_name_list_free(&replica_subs);
    if (replica_sieve) sync_sieve_list_free(&replica_sieve);
    if (replica_seen) sync_seen_list_free(&replica_seen);
    if (replica_quota) sync_quota_list_free(&replica_quota);

    return r;
}
