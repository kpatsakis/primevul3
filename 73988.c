static void cmd_login(char *tag, char *user)
{
    char userbuf[MAX_MAILBOX_BUFFER];
    char replybuf[MAX_MAILBOX_BUFFER];
    unsigned userlen;
    const char *canon_user = userbuf;
    const void *val;
    int c;
    struct buf passwdbuf;
    char *passwd;
    const char *reply = NULL;
    int r;
    int failedloginpause;

    if (imapd_userid) {
        eatline(imapd_in, ' ');
        prot_printf(imapd_out, "%s BAD Already logged in\r\n", tag);
        return;
    }

    r = imapd_canon_user(imapd_saslconn, NULL, user, 0,
                         SASL_CU_AUTHID | SASL_CU_AUTHZID, NULL,
                         userbuf, sizeof(userbuf), &userlen);

    if (r) {
        eatline(imapd_in, ' ');
        syslog(LOG_NOTICE, "badlogin: %s plaintext %s invalid user",
               imapd_clienthost, beautify_string(user));
        prot_printf(imapd_out, "%s NO %s\r\n", tag,
                    error_message(IMAP_INVALID_USER));
        return;
    }

    /* possibly disallow login */
    if (imapd_tls_required ||
        (!imapd_starttls_done && (extprops_ssf < 2) &&
         !config_getswitch(IMAPOPT_ALLOWPLAINTEXT) &&
         !is_userid_anonymous(canon_user))) {
        eatline(imapd_in, ' ');
        prot_printf(imapd_out, "%s NO Login only available under a layer\r\n",
                    tag);
        return;
    }

    memset(&passwdbuf,0,sizeof(struct buf));
    c = getastring(imapd_in, imapd_out, &passwdbuf);

    if(c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        buf_free(&passwdbuf);
        prot_printf(imapd_out,
                    "%s BAD Unexpected extra arguments to LOGIN\r\n",
                    tag);
        eatline(imapd_in, c);
        return;
    }

    passwd = passwdbuf.s;

    if (is_userid_anonymous(canon_user)) {
        if (config_getswitch(IMAPOPT_ALLOWANONYMOUSLOGIN)) {
            passwd = beautify_string(passwd);
            if (strlen(passwd) > 500) passwd[500] = '\0';
            syslog(LOG_NOTICE, "login: %s anonymous %s",
                   imapd_clienthost, passwd);
            reply = "Anonymous access granted";
            imapd_userid = xstrdup("anonymous");
        }
        else {
            syslog(LOG_NOTICE, "badlogin: %s anonymous login refused",
                   imapd_clienthost);
            prot_printf(imapd_out, "%s NO %s\r\n", tag,
                   error_message(IMAP_ANONYMOUS_NOT_PERMITTED));
            buf_free(&passwdbuf);
            return;
        }
    }
    else if ( nosaslpasswdcheck ) {
        snprintf(replybuf, sizeof(replybuf),
            "User logged in SESSIONID=<%s>", session_id());
        reply = replybuf;
        imapd_userid = xstrdup(canon_user);
        imapd_authstate = auth_newstate(canon_user);
        syslog(LOG_NOTICE, "login: %s %s%s nopassword%s %s", imapd_clienthost,
               imapd_userid, imapd_magicplus ? imapd_magicplus : "",
               imapd_starttls_done ? "+TLS" : "", reply);
    }
    else if ((r = sasl_checkpass(imapd_saslconn,
                                 canon_user,
                                 strlen(canon_user),
                                 passwd,
                                 strlen(passwd))) != SASL_OK) {
        syslog(LOG_NOTICE, "badlogin: %s plaintext %s %s",
               imapd_clienthost, canon_user, sasl_errdetail(imapd_saslconn));

        failedloginpause = config_getint(IMAPOPT_FAILEDLOGINPAUSE);
        if (failedloginpause != 0) {
            sleep(failedloginpause);
        }

        /* Don't allow user probing */
        if (r == SASL_NOUSER) r = SASL_BADAUTH;

        if ((reply = sasl_errstring(r, NULL, NULL)) != NULL) {
            prot_printf(imapd_out, "%s NO Login failed: %s\r\n", tag, reply);
        } else {
            prot_printf(imapd_out, "%s NO Login failed: %d\r\n", tag, r);
        }

        snmp_increment_args(AUTHENTICATION_NO, 1,
                            VARIABLE_AUTH, 0 /* hash_simple("LOGIN") */,
                            VARIABLE_LISTEND);
        buf_free(&passwdbuf);
        return;
    }
    else {
        r = sasl_getprop(imapd_saslconn, SASL_USERNAME, &val);

        if(r != SASL_OK) {
            if ((reply = sasl_errstring(r, NULL, NULL)) != NULL) {
                prot_printf(imapd_out, "%s NO Login failed: %s\r\n",
                            tag, reply);
            } else {
                prot_printf(imapd_out, "%s NO Login failed: %d\r\n", tag, r);
            }

            snmp_increment_args(AUTHENTICATION_NO, 1,
                                VARIABLE_AUTH, 0 /* hash_simple("LOGIN") */,
                                VARIABLE_LISTEND);
            buf_free(&passwdbuf);
            return;
        }

        snprintf(replybuf, sizeof(replybuf),
            "User logged in SESSIONID=<%s>", session_id());
        reply = replybuf;
        imapd_userid = xstrdup((const char *) val);
        snmp_increment_args(AUTHENTICATION_YES, 1,
                            VARIABLE_AUTH, 0 /*hash_simple("LOGIN") */,
                            VARIABLE_LISTEND);
        syslog(LOG_NOTICE, "login: %s %s%s plaintext%s %s", imapd_clienthost,
               imapd_userid, imapd_magicplus ? imapd_magicplus : "",
               imapd_starttls_done ? "+TLS" : "",
               reply ? reply : "");

        /* Apply penalty only if not under layer */
        if (!imapd_starttls_done) {
            int plaintextloginpause = config_getint(IMAPOPT_PLAINTEXTLOGINPAUSE);
            if (plaintextloginpause) {
                sleep(plaintextloginpause);
            }

            /* Fetch plaintext login nag message */
            plaintextloginalert = config_getstring(IMAPOPT_PLAINTEXTLOGINALERT);
        }
    }

    buf_free(&passwdbuf);

    if (checklimits(tag)) return;

    prot_printf(imapd_out, "%s OK [CAPABILITY ", tag);
    capa_response(CAPA_PREAUTH|CAPA_POSTAUTH);
    prot_printf(imapd_out, "] %s\r\n", reply);

    authentication_success();
}
