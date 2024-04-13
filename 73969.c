static int checklimits(const char *tag)
{
    struct proc_limits limits;

    limits.procname = "imapd";
    limits.clienthost = imapd_clienthost;
    limits.userid = imapd_userid;

    if (proc_checklimits(&limits)) {
        const char *sep = "";
        prot_printf(imapd_out, "%s NO Too many open connections (", tag);
        if (limits.maxhost) {
            prot_printf(imapd_out, "%s%d of %d from %s", sep,
                        limits.host, limits.maxhost, imapd_clienthost);
            sep = ", ";
        }
        if (limits.maxuser) {
            prot_printf(imapd_out, "%s%d of %d for %s", sep,
                        limits.user, limits.maxuser, imapd_userid);
        }
        prot_printf(imapd_out, ")\r\n");
        free(imapd_userid);
        imapd_userid = NULL;
        auth_freestate(imapd_authstate);
        imapd_authstate = NULL;
        return 1;
    }
    return 0;
}
