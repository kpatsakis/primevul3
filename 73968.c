static int catenate_url(const char *s, const char *cur_name, FILE *f,
                        unsigned *totalsize, const char **parseerr)
{
    struct imapurl url;
    struct index_state *state;
    uint32_t msgno;
    int r = 0, doclose = 0;
    unsigned long size = 0;

    r = imapurl_fromURL(&url, s);

    if (r) {
        *parseerr = "Improperly specified URL";
        r = IMAP_BADURL;
    } else if (url.server) {
        *parseerr = "Only relative URLs are supported";
        r = IMAP_BADURL;
#if 0
    } else if (url.server && strcmp(url.server, config_servername)) {
        *parseerr = "Cannot catenate messages from another server";
        r = IMAP_BADURL;
#endif
    } else if (!url.mailbox && !imapd_index && !cur_name) {
        *parseerr = "No mailbox is selected or specified";
        r = IMAP_BADURL;
    } else if (url.mailbox || (url.mailbox = cur_name)) {
        mbentry_t *mbentry = NULL;

        /* lookup the location of the mailbox */
        char *intname = mboxname_from_external(url.mailbox, &imapd_namespace, imapd_userid);
        r = mlookup(NULL, NULL, intname, &mbentry);

        if (!r && (mbentry->mbtype & MBTYPE_REMOTE)) {
            /* remote mailbox */
            struct backend *be;

            be = proxy_findserver(mbentry->server, &imap_protocol,
                                 proxy_userid, &backend_cached,
                                 &backend_current, &backend_inbox, imapd_in);
            if (be) {
                r = proxy_catenate_url(be, &url, f, &size, parseerr);
                if (*totalsize > UINT_MAX - size)
                    r = IMAP_MESSAGE_TOO_LARGE;
                else
                    *totalsize += size;
            }
            else
                r = IMAP_SERVER_UNAVAILABLE;

            free(url.freeme);
            mboxlist_entry_free(&mbentry);
            free(intname);

            return r;
        }

        mboxlist_entry_free(&mbentry);

        /* local mailbox */
        if (!r) {
            struct index_init init;
            memset(&init, 0, sizeof(init));
            init.userid = imapd_userid;
            init.authstate = imapd_authstate;
            init.out = imapd_out;
            r = index_open(intname, &init, &state);
            if (init.vanishedlist) seqset_free(init.vanishedlist);
        }
        if (!r) doclose = 1;

        if (!r && !(state->myrights & ACL_READ))
            r = (imapd_userisadmin || (state->myrights & ACL_LOOKUP)) ?
                IMAP_PERMISSION_DENIED : IMAP_MAILBOX_NONEXISTENT;

        if (r) {
            *parseerr = error_message(r);
            r = IMAP_BADURL;
        }

        free(intname);
    } else {
        state = imapd_index;
    }

    if (r) {
        /* nothing to do, handled up top */
    } else if (url.uidvalidity &&
               (state->mailbox->i.uidvalidity != url.uidvalidity)) {
        *parseerr = "Uidvalidity of mailbox has changed";
        r = IMAP_BADURL;
    } else if (!url.uid || !(msgno = index_finduid(state, url.uid)) ||
               (index_getuid(state, msgno) != url.uid)) {
        *parseerr = "No such message in mailbox";
        r = IMAP_BADURL;
    } else {
        /* Catenate message part to stage */
        struct protstream *s = prot_new(fileno(f), 1);

        r = index_urlfetch(state, msgno, 0, url.section,
                           url.start_octet, url.octet_count, s, &size);
        if (r == IMAP_BADURL)
            *parseerr = "No such message part";
        else if (!r) {
            if (*totalsize > UINT_MAX - size)
                r = IMAP_MESSAGE_TOO_LARGE;
            else
                *totalsize += size;
        }

        prot_flush(s);
        prot_free(s);

        /* XXX  do we want to try and validate the message like
           we do in message_copy_strict()? */
    }

    free(url.freeme);

    if (doclose) index_close(&state);

    return r;
}
