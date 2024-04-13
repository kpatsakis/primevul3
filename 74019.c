static void cmd_xfer(const char *tag, const char *name,
                     const char *toserver, const char *topart)
{
    int r = 0, partial_success = 0, mbox_count = 0;
    struct xfer_header *xfer = NULL;
    struct xfer_list list = { &imapd_namespace, imapd_userid, NULL, 0, NULL };
    struct xfer_item *item, *next;
    char *intname = NULL;

    /* administrators only please */
    /* however, proxys can do this, if their authzid is an admin */
    if (!imapd_userisadmin && !imapd_userisproxyadmin) {
        r = IMAP_PERMISSION_DENIED;
        goto done;
    }

    if (!strcmp(toserver, config_servername)) {
        r = IMAP_BAD_SERVER;
        goto done;
    }

    /* Build list of users/mailboxes to transfer */
    if (config_partitiondir(name)) {
        /* entire partition */
        list.part = name;
        mboxlist_findall(NULL, "*", 1, NULL, NULL, xfer_addmbox, &list);
    } else {
        /* mailbox pattern */
        mbname_t *mbname;

        intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);

        mbname = mbname_from_intname(intname);
        if (mbname_localpart(mbname) &&
            (mbname_isdeleted(mbname) || strarray_size(mbname_boxes(mbname)))) {
            /* targeted a user submailbox */
            list.allow_usersubs = 1;
        }
        mbname_free(&mbname);

        mboxlist_findall(NULL, intname, 1, NULL, NULL, xfer_addmbox, &list);
        free(intname);
    }

    r = xfer_init(toserver, &xfer);
    if (r) goto done;

    for (item = list.mboxes; item; item = next) {
        mbentry_t *mbentry = item->mbentry;

        /* NOTE: Since XFER can only be used by an admin, and we always connect
         * to the destination backend as an admin, we take advantage of the fact
         * that admins *always* use a consistent mailbox naming scheme.
         * So, 'name' should be used in any command we send to a backend, and
         * 'intname' is the internal name to be used for mupdate and findall.
         */

        r = 0;
        intname = mbentry->name;
        xfer->topart = xstrdup(topart ? topart : mbentry->partition);

        /* if we are not moving a user, just move the one mailbox */
        if (item->state != XFER_MOVING_USER) {

            syslog(LOG_INFO, "XFER: mailbox '%s' -> %s!%s",
                   mbentry->name, xfer->toserver, xfer->topart);

            /* is the selected mailbox the one we're moving? */
            if (!strcmpsafe(intname, index_mboxname(imapd_index))) {
                r = IMAP_MAILBOX_LOCKED;
                goto next;
            }

            /* we're moving this mailbox */
            xfer_addusermbox(mbentry, xfer);
            mbox_count++;

            r = do_xfer(xfer);
        } else {
            xfer->userid = mboxname_to_userid(intname);

            syslog(LOG_INFO, "XFER: user '%s' -> %s!%s",
                   xfer->userid, xfer->toserver, xfer->topart);

            if (!config_getswitch(IMAPOPT_ALLOWUSERMOVES)) {
                /* not configured to allow user moves */
                r = IMAP_MAILBOX_NOTSUPPORTED;
            } else if (!strcmp(xfer->userid, imapd_userid)) {
                /* don't move your own inbox, that could be troublesome */
                r = IMAP_MAILBOX_NOTSUPPORTED;
            } else if (!strncmpsafe(intname, index_mboxname(imapd_index),
                             strlen(intname))) {
                /* selected mailbox is in the namespace we're moving */
                r = IMAP_MAILBOX_LOCKED;
            }
            if (r) goto next;

            if (!xfer->use_replication) {
                /* set the quotaroot if needed */
                r = xfer_setquotaroot(xfer, intname);
                if (r) goto next;

                /* backport the seen file if needed */
                if (xfer->remoteversion < 12) {
                    r = seen_open(xfer->userid, SEEN_CREATE, &xfer->seendb);
                    if (r) goto next;
                }
            }

            r = mboxlist_usermboxtree(xfer->userid, xfer_addusermbox,
                                      xfer, MBOXTREE_DELETED);

            /* NOTE: mailboxes were added in reverse, so the inbox is
             * done last */
            r = do_xfer(xfer);
            if (r) goto next;

            /* this was a successful user move, and we need to delete
               certain user meta-data (but not seen state!) */
            syslog(LOG_INFO, "XFER: deleting user metadata");
            user_deletedata(xfer->userid, 0);
        }

      next:
        if (r) {
            if (xfer->userid)
                prot_printf(imapd_out, "* NO USER %s (%s)\r\n",
                            xfer->userid, error_message(r));
            else
                prot_printf(imapd_out, "* NO MAILBOX \"%s\" (%s)\r\n",
                            item->extname, error_message(r));
        } else {
            partial_success = 1;

            if (xfer->userid)
                prot_printf(imapd_out, "* OK USER %s\r\n", xfer->userid);
            else
                prot_printf(imapd_out, "* OK MAILBOX \"%s\"\r\n", item->extname);
        }
        prot_flush(imapd_out);

        mboxlist_entry_free(&mbentry);
        next = item->next;
        free(item);

        if (xfer->userid || mbox_count > 1000) {
            /* RESTART after each user or after every 1000 mailboxes */
            mbox_count = 0;

            sync_send_restart(xfer->be->out);
            r = sync_parse_response("RESTART", xfer->be->in, NULL);
            if (r) goto done;
        }

        xfer_cleanup(xfer);

        if (partial_success) r = 0;
    }

done:
    if (xfer) xfer_done(&xfer);

    imapd_check(NULL, 0);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag,
                    error_message(r));
    } else {
        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
    }

    return;
}
