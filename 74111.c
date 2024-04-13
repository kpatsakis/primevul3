static int xfer_undump(struct xfer_header *xfer)
{
    struct xfer_item *item;
    int r;
    mbentry_t *newentry;
    struct mailbox *mailbox = NULL;

    syslog(LOG_INFO, "XFER: dumping mailboxes to destination");

    for (item = xfer->items; item; item = item->next) {
        r = mailbox_open_irl(item->mbentry->name, &mailbox);
        if (r) {
            syslog(LOG_ERR,
                   "Failed to open mailbox %s for dump_mailbox() %s",
                   item->mbentry->name, error_message(r));
            return r;
        }

        /* Step 3.5: Set mailbox as MOVING on local server */
        /* XXX - this code is awful... need a sane way to manage mbentries */
        newentry = mboxlist_entry_create();
        newentry->name = xstrdupnull(item->mbentry->name);
        newentry->acl = xstrdupnull(item->mbentry->acl);
        newentry->server = xstrdupnull(xfer->toserver);
        newentry->partition = xstrdupnull(xfer->topart);
        newentry->mbtype = item->mbentry->mbtype|MBTYPE_MOVING;
        r = mboxlist_update(newentry, 1);
        mboxlist_entry_free(&newentry);

        if (r) {
            syslog(LOG_ERR,
                   "Could not move mailbox: %s, mboxlist_update() failed %s",
                   item->mbentry->name, error_message(r));
        }
        else item->state = XFER_LOCAL_MOVING;

        if (!r && xfer->seendb) {
            /* Backport the user's seendb on-the-fly */
            item->mailbox = mailbox;
            r = xfer_backport_seen_item(item, xfer->seendb);
            if (r) syslog(LOG_WARNING,
                          "Failed to backport seen state for mailbox '%s'",
                          item->mbentry->name);

            /* Need to close seendb before dumping Inbox (last item) */
            if (!item->next) seen_close(&xfer->seendb);
        }

        /* Step 4: Dump local -> remote */
        if (!r) {
            prot_printf(xfer->be->out, "D01 UNDUMP {" SIZE_T_FMT "+}\r\n%s ",
                        strlen(item->extname), item->extname);

            r = dump_mailbox(NULL, mailbox, 0, xfer->remoteversion,
                             xfer->be->in, xfer->be->out, imapd_authstate);
            if (r) {
                syslog(LOG_ERR,
                       "Could not move mailbox: %s, dump_mailbox() failed %s",
                       item->mbentry->name, error_message(r));
            }
        }

        mailbox_close(&mailbox);

        if (r) return r;

        r = getresult(xfer->be->in, "D01");
        if (r) {
            syslog(LOG_ERR, "Could not move mailbox: %s, UNDUMP failed %s",
                   item->mbentry->name, error_message(r));
            return r;
        }

        /* Step 5: Set ACL on remote */
        r = trashacl(xfer->be->in, xfer->be->out,
                     item->extname);
        if (r) {
            syslog(LOG_ERR, "Could not clear remote acl on %s",
                   item->mbentry->name);
            return r;
        }

        r = dumpacl(xfer->be->in, xfer->be->out,
                    item->extname, item->mbentry->acl);
        if (r) {
            syslog(LOG_ERR, "Could not set remote acl on %s",
                   item->mbentry->name);
            return r;
        }

        item->state = XFER_UNDUMPED;
    }

    return 0;
}
