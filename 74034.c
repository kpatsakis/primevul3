static int do_xfer(struct xfer_header *xfer)
{
    int r = 0;

    if (xfer->use_replication) {
        /* Initial non-blocking sync */
        r = xfer_initialsync(xfer);
        if (r) return r;
    }

    r = xfer_deactivate(xfer);

    if (!r) {
        if (xfer->use_replication) {
            /* Final sync with write locks on mailboxes */
            r = xfer_finalsync(xfer);
        }
        else {
            r = xfer_localcreate(xfer);
            if (!r) r = xfer_undump(xfer);
        }
    }

    if (r) {
        /* Something failed, revert back to local server */
        xfer_recover(xfer);
        return r;
    }

    /* Successful dump of all mailboxes to remote server.
     * Remove them locally and activate them on remote.
     * Note - we don't report errors if this fails! */
    xfer_delete(xfer);
    xfer_reactivate(xfer);

    return 0;
}
