static void cmd_syncrestore(const char *tag, struct dlist *kin,
                            struct sync_reserve_list *reserve_list)
{
    struct sync_state sync_state = {
        imapd_userid,
        imapd_userisadmin || imapd_userisproxyadmin,
        imapd_authstate,
        &imapd_namespace,
        imapd_out,
        0 /* local_only */
    };

    /* administrators only please */
    if (!imapd_userisadmin) {
        syslog(LOG_ERR, "SYNCERROR: invalid user %s trying to sync", imapd_userid);
        prot_printf(imapd_out, "%s NO only admininstrators may use sync commands\r\n", tag);
        return;
    }

    const char *resp = sync_restore(kin, reserve_list, &sync_state);
    prot_printf(imapd_out, "%s %s\r\n", tag, resp);

    /* Reset inactivity timer in case we spent a long time processing data */
    prot_resettimeout(imapd_in);
}
