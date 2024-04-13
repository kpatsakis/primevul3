static void warn_about_quota(const char *quotaroot)
{
    time_t now = time(NULL);
    struct quota q;
    int res;
    int r;
    int thresholds[QUOTA_NUMRESOURCES];
    int pc_threshold = config_getint(IMAPOPT_QUOTAWARN);
    int pc_usage;
    struct buf msg = BUF_INITIALIZER;
    static char lastqr[MAX_MAILBOX_PATH+1] = "";
    static time_t nextalert = 0;

    if (!quotaroot || !*quotaroot)
        return;     /* no quota, nothing to do */

    /* rate limit checks and warnings to every 10 min */
    if (!strcmp(quotaroot, lastqr) && now < nextalert)
        return;
    strlcpy(lastqr, quotaroot, sizeof(lastqr));
    nextalert = now + 600;

    quota_init(&q, quotaroot);
    r = quota_read(&q, NULL, 0);
    if (r)
        goto out;           /* failed to read */

    memset(thresholds, 0, sizeof(thresholds));
    thresholds[QUOTA_STORAGE] = config_getint(IMAPOPT_QUOTAWARNKB);
    thresholds[QUOTA_MESSAGE] = config_getint(IMAPOPT_QUOTAWARNMSG);
    thresholds[QUOTA_ANNOTSTORAGE] = config_getint(IMAPOPT_QUOTAWARNKB);

    for (res = 0 ; res < QUOTA_NUMRESOURCES ; res++) {
        if (q.limits[res] < 0)
            continue;       /* this resource is unlimited */
        buf_reset(&msg);

        if (thresholds[res] <= 0 ||
            thresholds[res] >= q.limits[res] ||
            q.useds[res] > ((quota_t) (q.limits[res] - thresholds[res])) * quota_units[res]) {

            pc_usage = (int)(((double) q.useds[res] * 100.0) /
                             (double) ((quota_t) q.limits[res] * quota_units[res]));

            if (q.useds[res] > (quota_t) q.limits[res] * quota_units[res])
                buf_printf(&msg, error_message(IMAP_NO_OVERQUOTA),
                           quota_names[res]);
            else if (pc_usage > pc_threshold)
                buf_printf(&msg, error_message(IMAP_NO_CLOSEQUOTA),
                           pc_usage, quota_names[res]);
        }

        if (msg.len)
            prot_printf(imapd_out, "* NO [ALERT] %s\r\n", buf_cstring(&msg));
    }

    buf_reset(&msg);
out:
    quota_free(&q);
}
