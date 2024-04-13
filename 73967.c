static void capa_response(int flags)
{
    const char *sasllist; /* the list of SASL mechanisms */
    int mechcount;
    int need_space = 0;
    int i;

    int lminus = config_getswitch(IMAPOPT_LITERALMINUS);

    for (i = 0; base_capabilities[i].str; i++) {
        const char *capa = base_capabilities[i].str;
        /* Filter capabilities if requested */
        if (capa_is_disabled(capa))
            continue;
        /* Don't show "MAILBOX-REFERRALS" if disabled by config */
        if (config_getswitch(IMAPOPT_PROXYD_DISABLE_MAILBOX_REFERRALS) &&
            !strcmp(capa, "MAILBOX-REFERRALS"))
            continue;
        /* Don't show if they're not shown at this level of login */
        if (!(base_capabilities[i].mask & flags))
            continue;
        /* cheap and nasty version of LITERAL- support - just say so */
        if (lminus && !strcmp(capa, "LITERAL+"))
            capa = "LITERAL-";
        /* print the capability */
        if (need_space) prot_putc(' ', imapd_out);
        else need_space = 1;
        prot_printf(imapd_out, "%s", base_capabilities[i].str);
    }

    if (config_mupdate_server) {
        prot_printf(imapd_out, " MUPDATE=mupdate://%s/", config_mupdate_server);
    }

    if (apns_enabled) {
        prot_printf(imapd_out, " XAPPLEPUSHSERVICE");
    }

    if (tls_enabled() && !imapd_starttls_done && !imapd_authstate) {
        prot_printf(imapd_out, " STARTTLS");
    }
    if (imapd_tls_required || imapd_authstate ||
        (!imapd_starttls_done && (extprops_ssf < 2) &&
         !config_getswitch(IMAPOPT_ALLOWPLAINTEXT))) {
        prot_printf(imapd_out, " LOGINDISABLED");
    }

    /* add the SASL mechs */
    if (!imapd_tls_required && (!imapd_authstate || saslprops.ssf) &&
        sasl_listmech(imapd_saslconn, NULL,
                      "AUTH=", " AUTH=",
                      !imapd_authstate ? " SASL-IR" : "", &sasllist,
                      NULL, &mechcount) == SASL_OK && mechcount > 0) {
        prot_printf(imapd_out, " %s", sasllist);
    } else {
        /* else don't show anything */
    }

    if (!(flags & CAPA_POSTAUTH)) return;

    if (config_getswitch(IMAPOPT_CONVERSATIONS))
        prot_printf(imapd_out, " XCONVERSATIONS");

#ifdef HAVE_ZLIB
    if (!imapd_compress_done && !imapd_tls_comp) {
        prot_printf(imapd_out, " COMPRESS=DEFLATE");
    }
#endif // HAVE_ZLIB

    for (i = 0 ; i < QUOTA_NUMRESOURCES ; i++)
        prot_printf(imapd_out, " X-QUOTA=%s", quota_names[i]);

    if (idle_enabled()) {
        prot_printf(imapd_out, " IDLE");
    }
}
