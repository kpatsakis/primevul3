static void cmd_authenticate(char *tag, char *authtype, char *resp)
{
    int sasl_result;

    const void *val;
    const char *ssfmsg = NULL;
    char replybuf[MAX_MAILBOX_BUFFER];
    const char *reply = NULL;

    const char *canon_user;

    int r;
    int failedloginpause;

    if (imapd_tls_required) {
        prot_printf(imapd_out,
                    "%s NO Authenticate only available under a layer\r\n", tag);
        return;
    }

    r = saslserver(imapd_saslconn, authtype, resp, "", "+ ", "",
                   imapd_in, imapd_out, &sasl_result, NULL);

    if (r) {
        const char *errorstring = NULL;

        switch (r) {
        case IMAP_SASL_CANCEL:
            prot_printf(imapd_out,
                        "%s BAD Client canceled authentication\r\n", tag);
            break;
        case IMAP_SASL_PROTERR:
            errorstring = prot_error(imapd_in);

            prot_printf(imapd_out,
                        "%s NO Error reading client response: %s\r\n",
                        tag, errorstring ? errorstring : "");
            break;
        default:
            /* failed authentication */
            syslog(LOG_NOTICE, "badlogin: %s %s [%s]",
                   imapd_clienthost, authtype, sasl_errdetail(imapd_saslconn));

            snmp_increment_args(AUTHENTICATION_NO, 1,
                                VARIABLE_AUTH, 0, /* hash_simple(authtype) */
                                VARIABLE_LISTEND);
            failedloginpause = config_getint(IMAPOPT_FAILEDLOGINPAUSE);
            if (failedloginpause != 0) {
                sleep(failedloginpause);
            }

            /* Don't allow user probing */
            if (sasl_result == SASL_NOUSER) sasl_result = SASL_BADAUTH;

            errorstring = sasl_errstring(sasl_result, NULL, NULL);
            if (errorstring) {
                prot_printf(imapd_out, "%s NO %s\r\n", tag, errorstring);
            } else {
                prot_printf(imapd_out, "%s NO Error authenticating\r\n", tag);
            }
        }

        reset_saslconn(&imapd_saslconn);
        return;
    }

    /* successful authentication */

    /* get the userid from SASL --- already canonicalized from
     * mysasl_proxy_policy()
     */
    sasl_result = sasl_getprop(imapd_saslconn, SASL_USERNAME, &val);
    if (sasl_result != SASL_OK) {
        prot_printf(imapd_out, "%s NO weird SASL error %d SASL_USERNAME\r\n",
                    tag, sasl_result);
        syslog(LOG_ERR, "weird SASL error %d getting SASL_USERNAME",
               sasl_result);
        reset_saslconn(&imapd_saslconn);
        return;
    }
    canon_user = (const char *) val;

    /* If we're proxying, the authzid may contain a magic plus,
       so re-canonify it */
    if (config_getswitch(IMAPOPT_IMAPMAGICPLUS) && strchr(canon_user, '+')) {
        char userbuf[MAX_MAILBOX_BUFFER];
        unsigned userlen;

        sasl_result = imapd_canon_user(imapd_saslconn, NULL, canon_user, 0,
                                       SASL_CU_AUTHID | SASL_CU_AUTHZID,
                                       NULL, userbuf, sizeof(userbuf), &userlen);
        if (sasl_result != SASL_OK) {
            prot_printf(imapd_out,
                        "%s NO SASL canonification error %d\r\n",
                        tag, sasl_result);
            reset_saslconn(&imapd_saslconn);
            return;
        }

        imapd_userid = xstrdup(userbuf);
    } else {
        imapd_userid = xstrdup(canon_user);
    }

    snprintf(replybuf, sizeof(replybuf),
        "User logged in SESSIONID=<%s>", session_id());
    reply = replybuf;
    syslog(LOG_NOTICE, "login: %s %s%s %s%s %s", imapd_clienthost,
           imapd_userid, imapd_magicplus ? imapd_magicplus : "",
           authtype, imapd_starttls_done ? "+TLS" : "", reply);

    sasl_getprop(imapd_saslconn, SASL_SSF, &val);
    saslprops.ssf = *((sasl_ssf_t *) val);

    /* really, we should be doing a sasl_getprop on SASL_SSF_EXTERNAL,
       but the current libsasl doesn't allow that. */
    if (imapd_starttls_done) {
        switch(saslprops.ssf) {
        case 0: ssfmsg = "tls protection"; break;
        case 1: ssfmsg = "tls plus integrity protection"; break;
        default: ssfmsg = "tls plus privacy protection"; break;
        }
    } else {
        switch(saslprops.ssf) {
        case 0: ssfmsg = "no protection"; break;
        case 1: ssfmsg = "integrity protection"; break;
        default: ssfmsg = "privacy protection"; break;
        }
    }

    snmp_increment_args(AUTHENTICATION_YES, 1,
                        VARIABLE_AUTH, 0, /* hash_simple(authtype) */
                        VARIABLE_LISTEND);

    if (checklimits(tag)) {
        reset_saslconn(&imapd_saslconn);
        return;
    }

    if (!saslprops.ssf) {
        prot_printf(imapd_out, "%s OK [CAPABILITY ", tag);
        capa_response(CAPA_PREAUTH|CAPA_POSTAUTH);
        prot_printf(imapd_out, "] Success (%s) SESSIONID=<%s>\r\n",
                    ssfmsg, session_id());
    } else {
        prot_printf(imapd_out, "%s OK Success (%s) SESSIONID=<%s>\r\n",
                    tag, ssfmsg, session_id());
    }

    prot_setsasl(imapd_in,  imapd_saslconn);
    prot_setsasl(imapd_out, imapd_saslconn);

    authentication_success();
}
