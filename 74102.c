static void xfer_done(struct xfer_header **xferptr)
{
    struct xfer_header *xfer = *xferptr;

    syslog(LOG_INFO, "XFER: disconnecting from servers");

    free(xfer->toserver);

    buf_free(&xfer->tagbuf);

    xfer_cleanup(xfer);

    free(xfer);

    *xferptr = NULL;
}
