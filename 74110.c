static int xfer_setquotaroot(struct xfer_header *xfer, const char *mboxname)
{
    struct quota q;
    int r;

    syslog(LOG_INFO, "XFER: setting quota root %s", mboxname);

    quota_init(&q, mboxname);
    r = quota_read(&q, NULL, 0);
    if (r == IMAP_QUOTAROOT_NONEXISTENT) return 0;
    if (r) return r;

    /* note use of + to force the setting of a nonexistant
     * quotaroot */
    char *extname = mboxname_to_external(mboxname, &imapd_namespace, imapd_userid);
    prot_printf(xfer->be->out, "Q01 SETQUOTA {" SIZE_T_FMT "+}\r\n+%s ",
                strlen(extname)+1, extname);
    free(extname);
    print_quota_limits(xfer->be->out, &q);
    prot_printf(xfer->be->out, "\r\n");
    quota_free(&q);

    r = getresult(xfer->be->in, "Q01");
    if (r) syslog(LOG_ERR,
                  "Could not move mailbox: %s, " \
                  "failed setting initial quota root\r\n",
                  mboxname);
    return r;
}
