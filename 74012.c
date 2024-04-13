static void cmd_urlfetch(char *tag)
{
    struct mboxkey *mboxkey_db;
    int c, r, doclose;
    static struct buf arg, param;
    struct imapurl url;
    struct index_state *state;
    uint32_t msgno;
    mbentry_t *mbentry = NULL;
    time_t now = time(NULL);
    unsigned extended, params;

    prot_printf(imapd_out, "* URLFETCH");

    do {
        char *intname = NULL;
        extended = params = 0;

        /* See if its an extended URLFETCH */
        c = prot_getc(imapd_in);
        if (c == '(') extended = 1;
        else prot_ungetc(c, imapd_in);

        c = getastring(imapd_in, imapd_out, &arg);
        (void)prot_putc(' ', imapd_out);
        prot_printstring(imapd_out, arg.s);

        if (extended) {
            while (c == ' ') {
                c = getword(imapd_in, &param);

                ucase(param.s);
                if (!strcmp(param.s, "BODY")) {
                    if (params & (URLFETCH_BODY | URLFETCH_BINARY)) goto badext;
                    params |= URLFETCH_BODY;
                } else if (!strcmp(param.s, "BINARY")) {
                    if (params & (URLFETCH_BODY | URLFETCH_BINARY)) goto badext;
                    params |= URLFETCH_BINARY;
                } else if (!strcmp(param.s, "BODYPARTSTRUCTURE")) {
                    if (params & URLFETCH_BODYPARTSTRUCTURE) goto badext;
                    params |= URLFETCH_BODYPARTSTRUCTURE;
                } else {
                    goto badext;
                }
            }

            if (c != ')') goto badext;
            c = prot_getc(imapd_in);
        }

        doclose = 0;
        r = imapurl_fromURL(&url, arg.s);

        /* validate the URL */
        if (r || !url.user || !url.server || !url.mailbox || !url.uid ||
            (url.section && !*url.section) ||
            (url.urlauth.access && !(url.urlauth.mech && url.urlauth.token))) {
            /* missing info */
            r = IMAP_BADURL;
        } else if (strcmp(url.server, config_servername)) {
            /* wrong server */
            r = IMAP_BADURL;
        } else if (url.urlauth.expire &&
                   url.urlauth.expire < mktime(gmtime(&now))) {
            /* expired */
            r = IMAP_BADURL;
        } else if (url.urlauth.access) {
            /* check mechanism & authorization */
            int authorized = 0;

            if (!strcasecmp(url.urlauth.mech, "INTERNAL")) {
                if (!strncasecmp(url.urlauth.access, "submit+", 7) &&
                    global_authisa(imapd_authstate, IMAPOPT_SUBMITSERVERS)) {
                    /* authorized submit server */
                    authorized = 1;
                } else if (!strncasecmp(url.urlauth.access, "user+", 5) &&
                           !strcmp(url.urlauth.access+5, imapd_userid)) {
                    /* currently authorized user */
                    authorized = 1;
                } else if (!strcasecmp(url.urlauth.access, "authuser") &&
                           strcmp(imapd_userid, "anonymous")) {
                    /* any non-anonymous authorized user */
                    authorized = 1;
                } else if (!strcasecmp(url.urlauth.access, "anonymous")) {
                    /* anyone */
                    authorized = 1;
                }
            }

            if (!authorized) r = IMAP_BADURL;
        }

        if (r) goto err;

        intname = mboxname_from_external(url.mailbox, &imapd_namespace, url.user);
        r = mlookup(NULL, NULL, intname, &mbentry);
        if (r) goto err;

        if ((mbentry->mbtype & MBTYPE_REMOTE)) {
            /* remote mailbox */
            struct backend *be;

            be = proxy_findserver(mbentry->server, &imap_protocol,
                                  proxy_userid, &backend_cached,
                                  &backend_current, &backend_inbox, imapd_in);
            if (!be) {
                r = IMAP_SERVER_UNAVAILABLE;
            } else {
                /* XXX  proxy command to backend */
            }

            free(url.freeme);

            mboxlist_entry_free(&mbentry);
            free(intname);

            continue;
        }

        mboxlist_entry_free(&mbentry);

        /* local mailbox */
        if (url.urlauth.token) {
            /* validate the URLAUTH token */

            /* yes, this is evil, in-place conversion from hex
             * to binary */
            if (hex_to_bin(url.urlauth.token, 0,
                (unsigned char *) url.urlauth.token) < 1) {
                r = IMAP_BADURL;
                break;
            }

            /* first byte is the algorithm used to create token */
            switch (url.urlauth.token[0]) {
            case URLAUTH_ALG_HMAC_SHA1: {
                const char *key;
                size_t keylen;
                unsigned char vtoken[EVP_MAX_MD_SIZE];
                unsigned int vtoken_len;

                r = mboxkey_open(url.user, 0, &mboxkey_db);
                if (r) break;

                r = mboxkey_read(mboxkey_db, intname, &key, &keylen);
                if (r) break;

                HMAC(EVP_sha1(), key, keylen, (unsigned char *) arg.s,
                     url.urlauth.rump_len, vtoken, &vtoken_len);
                mboxkey_close(mboxkey_db);

                if (memcmp(vtoken, url.urlauth.token+1, vtoken_len)) {
                    r = IMAP_BADURL;
                }

                break;
            }
            default:
                r = IMAP_BADURL;
                break;
            }
        }
        if (r) goto err;

        if (!strcmp(index_mboxname(imapd_index), intname)) {
            state = imapd_index;
        }
        else {
            /* not the currently selected mailbox, so try to open it */

            r = index_open(intname, NULL, &state);
            if (!r)
                doclose = 1;

            if (!r && !url.urlauth.access &&
                !(state->myrights & ACL_READ)) {
                r = (imapd_userisadmin ||
                     (state->myrights & ACL_LOOKUP)) ?
                    IMAP_PERMISSION_DENIED : IMAP_MAILBOX_NONEXISTENT;
            }
        }
        if (r) goto err;

        if (url.uidvalidity &&
           (state->mailbox->i.uidvalidity != url.uidvalidity)) {
            r = IMAP_BADURL;
        } else if (!url.uid || !(msgno = index_finduid(state, url.uid)) ||
                   (index_getuid(state, msgno) != url.uid)) {
            r = IMAP_BADURL;
        } else {
            r = index_urlfetch(state, msgno, params, url.section,
                               url.start_octet, url.octet_count,
                               imapd_out, NULL);
        }

        if (doclose)
            index_close(&state);

    err:
        free(url.freeme);

        if (r) prot_printf(imapd_out, " NIL");
        free(intname);

    } while (c == ' ');

    prot_printf(imapd_out, "\r\n");

    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out,
                    "%s BAD Unexpected extra arguments to URLFETCH\r\n", tag);
        eatline(imapd_in, c);
    }
    else {
        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
    }
    return;

  badext:
    prot_printf(imapd_out, " NIL\r\n");
    prot_printf(imapd_out,
                "%s BAD Invalid extended URLFETCH parameters\r\n", tag);
    eatline(imapd_in, c);
}
