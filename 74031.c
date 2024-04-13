static int do_xbackup(const char *channel,
                      const ptrarray_t *list)
{
    sasl_callback_t *cb = NULL;
    struct backend *backend = NULL;
    const char *hostname;
    const char *port;
    unsigned sync_flags = 0; // FIXME ??
    int partial_success = 0;
    int mbox_count = 0;
    int i, r;

    hostname = sync_get_config(channel, "sync_host");
    if (!hostname) {
        syslog(LOG_ERR, "XBACKUP: couldn't find hostname for channel '%s'", channel);
        return IMAP_BAD_SERVER;
    }

    port = sync_get_config(channel, "sync_port");
    if (port) csync_protocol.service = port;

    cb = mysasl_callbacks(NULL,
                          sync_get_config(channel, "sync_authname"),
                          sync_get_config(channel, "sync_realm"),
                          sync_get_config(channel, "sync_password"));

    syslog(LOG_INFO, "XBACKUP: connecting to server '%s' for channel '%s'",
                     hostname, channel);

    backend = backend_connect(NULL, hostname, &csync_protocol, NULL, cb, NULL, -1);
    if (!backend) {
        syslog(LOG_ERR, "XBACKUP: failed to connect to server '%s' for channel '%s'",
                        hostname, channel);
        return IMAP_SERVER_UNAVAILABLE;
    }

    free_callbacks(cb);
    cb = NULL;

    for (i = 0; i < list->count; i++) {
        const mbname_t *mbname = ptrarray_nth(list, i);
        if (!mbname) continue;
        const char *userid = mbname_userid(mbname);
        const char *intname = mbname_intname(mbname);

        if (userid) {
            syslog(LOG_INFO, "XBACKUP: replicating user %s", userid);

            r = sync_do_user(userid, NULL, backend, /*channelp*/ NULL, sync_flags);
        }
        else {
            struct sync_name_list *mboxname_list = sync_name_list_create();

            syslog(LOG_INFO, "XBACKUP: replicating mailbox %s", intname);
            sync_name_list_add(mboxname_list, intname);

            r = sync_do_mailboxes(mboxname_list, NULL, backend,
                                  /*channelp*/ NULL, sync_flags);
            mbox_count++;
            sync_name_list_free(&mboxname_list);
        }

        if (r) {
            prot_printf(imapd_out, "* NO %s %s (%s)\r\n",
                        userid ? "USER" : "MAILBOX",
                        userid ? userid : intname,
                        error_message(r));
        }
        else {
            partial_success++;
            prot_printf(imapd_out, "* OK %s %s\r\n",
                        userid ? "USER" : "MAILBOX",
                        userid ? userid : intname);
        }
        prot_flush(imapd_out);

        /* send RESTART after each user, or 1000 mailboxes */
        if (!r && i < list->count - 1 && (userid || mbox_count >= 1000)) {
            mbox_count = 0;

            sync_send_restart(backend->out);
            r = sync_parse_response("RESTART", backend->in, NULL);
            if (r) goto done;
        }
    }

    /* send a final RESTART */
    sync_send_restart(backend->out);
    sync_parse_response("RESTART", backend->in, NULL);

    if (partial_success) r = 0;

done:
    backend_disconnect(backend);
    free(backend);

    return r;
}
