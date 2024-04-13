static int xfer_mupdate(int isactivate,
                        const char *mboxname, const char *part,
                        const char *servername, const char *acl)
{
    char buf[MAX_PARTITION_LEN+HOSTNAME_SIZE+2];
    int retry = 0;
    int r = 0;

    /* no mupdate handle */
    if (!mupdate_h) return 0;

    snprintf(buf, sizeof(buf), "%s!%s", servername, part);

retry:
    /* make the change */
    if (isactivate)
        r = mupdate_activate(mupdate_h, mboxname, buf, acl);
    else
        r = mupdate_deactivate(mupdate_h, mboxname, buf);

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

    return r;
}
