static void cmd_starttls(char *tag, int imaps)
{
    int result;
    int *layerp;

    char *auth_id;
    sasl_ssf_t ssf;

    /* SASL and openssl have different ideas about whether ssf is signed */
    layerp = (int *) &ssf;

    if (imapd_starttls_done == 1)
    {
        prot_printf(imapd_out, "%s NO TLS already active\r\n", tag);
        return;
    }

    result=tls_init_serverengine("imap",
                                 5,        /* depth to verify */
                                 !imaps,   /* can client auth? */
                                 NULL);

    if (result == -1) {

        syslog(LOG_ERR, "error initializing TLS");

        if (imaps == 0) {
            prot_printf(imapd_out, "%s NO Error initializing TLS\r\n", tag);
        } else {
            shut_down(0);
        }

        return;
    }

    if (imaps == 0)
    {
        prot_printf(imapd_out, "%s OK Begin TLS negotiation now\r\n", tag);
        /* must flush our buffers before starting tls */
        prot_flush(imapd_out);
    }

    result=tls_start_servertls(0, /* read */
                               1, /* write */
                               imaps ? 180 : imapd_timeout,
                               layerp,
                               &auth_id,
                               &tls_conn);

    /* if error */
    if (result==-1) {
        if (imaps == 0) {
            prot_printf(imapd_out, "%s NO Starttls negotiation failed\r\n", tag);
            syslog(LOG_NOTICE, "STARTTLS negotiation failed: %s", imapd_clienthost);
            return;
        } else {
            syslog(LOG_NOTICE, "imaps TLS negotiation failed: %s", imapd_clienthost);
            shut_down(0);
        }
    }

    /* tell SASL about the negotiated layer */
    result = sasl_setprop(imapd_saslconn, SASL_SSF_EXTERNAL, &ssf);
    if (result == SASL_OK) {
        saslprops.ssf = ssf;

        result = sasl_setprop(imapd_saslconn, SASL_AUTH_EXTERNAL, auth_id);
    }
    if (result != SASL_OK) {
        syslog(LOG_NOTICE, "sasl_setprop() failed: cmd_starttls()");
        if (imaps == 0) {
            fatal("sasl_setprop() failed: cmd_starttls()", EC_TEMPFAIL);
        } else {
            shut_down(0);
        }
    }

    if(saslprops.authid) {
        free(saslprops.authid);
        saslprops.authid = NULL;
    }
    if(auth_id)
        saslprops.authid = xstrdup(auth_id);

    /* tell the prot layer about our new layers */
    prot_settls(imapd_in, tls_conn);
    prot_settls(imapd_out, tls_conn);

    imapd_starttls_done = 1;
    imapd_tls_required = 0;

#if (OPENSSL_VERSION_NUMBER >= 0x0090800fL)
    imapd_tls_comp = (void *) SSL_get_current_compression(tls_conn);
#endif
}
