static void imapd_reset(void)
{
    int i;
    int bytes_in = 0;
    int bytes_out = 0;

    proc_cleanup();

    /* close backend connections */
    i = 0;
    while (backend_cached && backend_cached[i]) {
        proxy_downserver(backend_cached[i]);
        if (backend_cached[i]->last_result.s) {
            free(backend_cached[i]->last_result.s);
        }
        free(backend_cached[i]);
        i++;
    }
    if (backend_cached) free(backend_cached);
    backend_cached = NULL;
    backend_inbox = backend_current = NULL;
    if (mupdate_h) mupdate_disconnect(&mupdate_h);
    mupdate_h = NULL;
    proxy_cmdcnt = 0;
    disable_referrals = 0;
    supports_referrals = 0;

    if (imapd_index) index_close(&imapd_index);

    if (imapd_in) {
        /* Flush the incoming buffer */
        prot_NONBLOCK(imapd_in);
        prot_fill(imapd_in);
        bytes_in = prot_bytes_in(imapd_in);
        prot_free(imapd_in);
    }

    if (imapd_out) {
        /* Flush the outgoing buffer */
        prot_flush(imapd_out);
        bytes_out = prot_bytes_out(imapd_out);
        prot_free(imapd_out);
    }

    if (config_auditlog)
        syslog(LOG_NOTICE, "auditlog: traffic sessionid=<%s> bytes_in=<%d> bytes_out=<%d>",
                           session_id(), bytes_in, bytes_out);

    imapd_in = imapd_out = NULL;

    if (protin) protgroup_reset(protin);

#ifdef HAVE_SSL
    if (tls_conn) {
        if (tls_reset_servertls(&tls_conn) == -1) {
            fatal("tls_reset() failed", EC_TEMPFAIL);
        }
        tls_conn = NULL;
    }
#endif

    cyrus_reset_stdio();

    imapd_clienthost = "[local]";
    if (imapd_logfd != -1) {
        close(imapd_logfd);
        imapd_logfd = -1;
    }
    if (imapd_userid != NULL) {
        free(imapd_userid);
        imapd_userid = NULL;
    }
    if (proxy_userid != NULL) {
        free(proxy_userid);
        proxy_userid = NULL;
    }
    if (imapd_magicplus != NULL) {
        free(imapd_magicplus);
        imapd_magicplus = NULL;
    }
    if (imapd_authstate) {
        auth_freestate(imapd_authstate);
        imapd_authstate = NULL;
    }
    imapd_userisadmin = 0;
    imapd_userisproxyadmin = 0;
    client_capa = 0;
    if (imapd_saslconn) {
        sasl_dispose(&imapd_saslconn);
        free(imapd_saslconn);
        imapd_saslconn = NULL;
    }
    imapd_compress_done = 0;
    imapd_tls_comp = NULL;
    imapd_starttls_done = 0;
    plaintextloginalert = NULL;

    if(saslprops.iplocalport) {
        free(saslprops.iplocalport);
        saslprops.iplocalport = NULL;
    }
    if(saslprops.ipremoteport) {
        free(saslprops.ipremoteport);
        saslprops.ipremoteport = NULL;
    }
    if(saslprops.authid) {
        free(saslprops.authid);
        saslprops.authid = NULL;
    }
    saslprops.ssf = 0;

    clear_id();
}
