static void cmd_mupdatepush(char *tag, char *name)
{
    int r = 0, retry = 0;
    mbentry_t *mbentry = NULL;
    char buf[MAX_PARTITION_LEN + HOSTNAME_SIZE + 2];
    char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);

    if (!imapd_userisadmin) {
        r = IMAP_PERMISSION_DENIED;
        goto done;
    }
    if (!config_mupdate_server) {
        r = IMAP_SERVER_UNAVAILABLE;
        goto done;
    }

    r = mlookup(tag, name, intname, &mbentry);
    if (r) goto done;

    /* Push mailbox to mupdate server */
    if (!mupdate_h) {
        syslog(LOG_INFO, "XFER: connecting to mupdate '%s'",
               config_mupdate_server);

        r = mupdate_connect(config_mupdate_server, NULL, &mupdate_h, NULL);
        retry = 1;
        if (r) {
            syslog(LOG_INFO, "Failed to connect to mupdate '%s'",
                   config_mupdate_server);
            goto done;
        }
    }

    snprintf(buf, sizeof(buf), "%s!%s",
             config_servername, mbentry->partition);

  retry:
    r = mupdate_activate(mupdate_h, intname, buf, mbentry->acl);

    if (r && !retry) {
        syslog(LOG_INFO, "MUPDATE: lost connection, retrying");
        mupdate_disconnect(&mupdate_h);
        r = mupdate_connect(config_mupdate_server, NULL, &mupdate_h, NULL);
        if (r) {
            syslog(LOG_INFO, "Failed to connect to mupdate '%s'",
                   config_mupdate_server);
        }
        else {
            retry = 1;
            goto retry;
        }
    }

done:
    mboxlist_entry_free(&mbentry);
    free(intname);
    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
    }
    else {
        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
    }
}
