static int xfer_init(const char *toserver, struct xfer_header **xferptr)
{
    struct xfer_header *xfer = xzmalloc(sizeof(struct xfer_header));
    int r;

    syslog(LOG_INFO, "XFER: connecting to server '%s'", toserver);

    /* Get a connection to the remote backend */
    xfer->be = proxy_findserver(toserver, &imap_protocol, "", &backend_cached,
                                NULL, NULL, imapd_in);
    if (!xfer->be) {
        syslog(LOG_ERR, "Failed to connect to server '%s'", toserver);
        r = IMAP_SERVER_UNAVAILABLE;
        goto fail;
    }

    xfer->remoteversion = backend_version(xfer->be);
    if (xfer->be->capability & CAPA_REPLICATION) {
        syslog(LOG_INFO, "XFER: destination supports replication");
        xfer->use_replication = 1;

        /* attach our IMAP tag buffer to our protstreams as userdata */
        xfer->be->in->userdata = xfer->be->out->userdata = &xfer->tagbuf;
    }

    xfer->toserver = xstrdup(toserver);
    xfer->topart = NULL;
    xfer->seendb = NULL;

    /* connect to mupdate server if configured */
    if (config_mupdate_server && !mupdate_h) {
        syslog(LOG_INFO, "XFER: connecting to mupdate '%s'",
               config_mupdate_server);

        r = mupdate_connect(config_mupdate_server, NULL, &mupdate_h, NULL);
        if (r) {
            syslog(LOG_INFO, "Failed to connect to mupdate '%s'",
                   config_mupdate_server);
            goto fail;
        }
    }

    *xferptr = xfer;
    return 0;

fail:
    xfer_done(&xfer);
    return r;
}
