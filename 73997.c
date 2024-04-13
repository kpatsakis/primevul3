static void cmd_select(char *tag, char *cmd, char *name)
{
    int c;
    int r = 0;
    int doclose = 0;
    mbentry_t *mbentry = NULL;
    struct backend *backend_next = NULL;
    struct index_init init;
    int wasopen = 0;
    struct vanished_params *v = &init.vanished;

    memset(&init, 0, sizeof(struct index_init));

    c = prot_getc(imapd_in);
    if (c == ' ') {
        static struct buf arg, parm1, parm2;

        c = prot_getc(imapd_in);
        if (c != '(') goto badlist;

        c = getword(imapd_in, &arg);
        if (arg.s[0] == '\0') goto badlist;
        for (;;) {
            ucase(arg.s);
            if (!strcmp(arg.s, "CONDSTORE")) {
                client_capa |= CAPA_CONDSTORE;
            }
            else if ((client_capa & CAPA_QRESYNC) &&
                     !strcmp(arg.s, "QRESYNC")) {
                char *p;

                if (c != ' ') goto badqresync;
                c = prot_getc(imapd_in);
                if (c != '(') goto badqresync;
                c = getastring(imapd_in, imapd_out, &arg);
                v->uidvalidity = strtoul(arg.s, &p, 10);
                if (*p || !v->uidvalidity || v->uidvalidity == ULONG_MAX) goto badqresync;
                if (c != ' ') goto badqresync;
                c = getmodseq(imapd_in, &v->modseq);
                if (c == EOF) goto badqresync;
                if (c == ' ') {
                    c = prot_getc(imapd_in);
                    if (c != '(') {
                        /* optional UID sequence */
                        prot_ungetc(c, imapd_in);
                        c = getword(imapd_in, &arg);
                        if (!imparse_issequence(arg.s)) goto badqresync;
                        v->sequence = arg.s;
                        if (c == ' ') {
                            c = prot_getc(imapd_in);
                            if (c != '(') goto badqresync;
                        }
                    }
                    if (c == '(') {
                        /* optional sequence match data */
                        c = getword(imapd_in, &parm1);
                        if (!imparse_issequence(parm1.s)) goto badqresync;
                        v->match_seq = parm1.s;
                        if (c != ' ') goto badqresync;
                        c = getword(imapd_in, &parm2);
                        if (!imparse_issequence(parm2.s)) goto badqresync;
                        v->match_uid = parm2.s;
                        if (c != ')') goto badqresync;
                        c = prot_getc(imapd_in);
                    }
                }
                if (c != ')') goto badqresync;
                c = prot_getc(imapd_in);
            }
            else if (!strcmp(arg.s, "ANNOTATE")) {
                /*
                 * RFC5257 requires us to parse this keyword, which
                 * indicates that the client wants unsolicited
                 * ANNOTATION responses in this session, but we don't
                 * actually have to do anything with it, so we won't.
                 */
                ;
            }
            else {
                prot_printf(imapd_out, "%s BAD Invalid %s modifier %s\r\n",
                            tag, cmd, arg.s);
                eatline(imapd_in, c);
                return;
            }

            if (c == ' ') c = getword(imapd_in, &arg);
            else break;
        }

        if (c != ')') {
            prot_printf(imapd_out,
                        "%s BAD Missing close parenthesis in %s\r\n", tag, cmd);
            eatline(imapd_in, c);
            return;
        }

        c = prot_getc(imapd_in);
    }
    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out,
                    "%s BAD Unexpected extra arguments to %s\r\n", tag, cmd);
        eatline(imapd_in, c);
        return;
    }

    if (imapd_index) {
        index_close(&imapd_index);
        wasopen = 1;
    }

    if (backend_current) {
        /* remove backend_current from the protgroup */
        protgroup_delete(protin, backend_current->in);
        wasopen = 1;
    }

    char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);
    r = mlookup(tag, name, intname, &mbentry);
    if (r == IMAP_MAILBOX_MOVED) {
        free(intname);
        return;
    }

    if (!r && (mbentry->mbtype & MBTYPE_REMOTE)) {
        char mytag[128];

        if (supports_referrals) {
            imapd_refer(tag, mbentry->server, name);
            mboxlist_entry_free(&mbentry);
            free(intname);
            return;
        }

        backend_next = proxy_findserver(mbentry->server, &imap_protocol,
                                        proxy_userid, &backend_cached,
                                        &backend_current, &backend_inbox,
                                        imapd_in);
        if (!backend_next) r = IMAP_SERVER_UNAVAILABLE;

        if (backend_current && backend_current != backend_next) {
            /* switching servers; flush old server output */
            proxy_gentag(mytag, sizeof(mytag));
            prot_printf(backend_current->out, "%s Unselect\r\n", mytag);
            /* do not fatal() here, because we don't really care about this
             * server anymore anyway */
            pipe_until_tag(backend_current, mytag, 1);
        }
        backend_current = backend_next;

        if (r) {
            prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
            mboxlist_entry_free(&mbentry);
            free(intname);
            return;
        }

        if (client_capa) {
            /* Enable client capabilities on new backend */
            proxy_gentag(mytag, sizeof(mytag));
            prot_printf(backend_current->out, "%s Enable", mytag);
            if (client_capa & CAPA_QRESYNC)
                prot_printf(backend_current->out, " Qresync");
            else if (client_capa & CAPA_CONDSTORE)
                prot_printf(backend_current->out, " Condstore");
            prot_printf(backend_current->out, "\r\n");
            pipe_until_tag(backend_current, mytag, 0);
        }

        /* Send SELECT command to backend */
        prot_printf(backend_current->out, "%s %s {" SIZE_T_FMT "+}\r\n%s",
                    tag, cmd, strlen(name), name);
        if (v->uidvalidity) {
            prot_printf(backend_current->out, " (QRESYNC (%lu " MODSEQ_FMT,
                        v->uidvalidity, v->modseq);
            if (v->sequence) {
                prot_printf(backend_current->out, " %s", v->sequence);
            }
            if (v->match_seq && v->match_uid) {
                prot_printf(backend_current->out, " (%s %s)",
                            v->match_seq, v->match_uid);
            }
            prot_printf(backend_current->out, "))");
        }
        prot_printf(backend_current->out, "\r\n");

        switch (pipe_including_tag(backend_current, tag, 0)) {
        case PROXY_OK:
            syslog(LOG_DEBUG, "open: user %s opened %s on %s",
                   imapd_userid, name, mbentry->server);

            /* add backend_current to the protgroup */
            protgroup_insert(protin, backend_current->in);
            break;
        default:
            syslog(LOG_DEBUG, "open: user %s failed to open %s", imapd_userid,
                   name);
            /* not successfully selected */
            backend_current = NULL;
            break;
        }

        mboxlist_entry_free(&mbentry);
        free(intname);

        return;
    }

    mboxlist_entry_free(&mbentry);

    /* local mailbox */
    if (backend_current) {
      char mytag[128];

      /* switching servers; flush old server output */
      proxy_gentag(mytag, sizeof(mytag));
      prot_printf(backend_current->out, "%s Unselect\r\n", mytag);
      /* do not fatal() here, because we don't really care about this
       * server anymore anyway */
      pipe_until_tag(backend_current, mytag, 1);
    }
    backend_current = NULL;

    if (wasopen) prot_printf(imapd_out, "* OK [CLOSED] Ok\r\n");

    init.userid = imapd_userid;
    init.authstate = imapd_authstate;
    init.out = imapd_out;
    init.examine_mode = cmd[0] == 'E';
    init.select = 1;
    if (!strcasecmpsafe(imapd_magicplus, "+dav")) init.want_dav = 1;

    r = index_open(intname, &init, &imapd_index);
    if (!r) doclose = 1;

    if (!r && !index_hasrights(imapd_index, ACL_READ)) {
        r = (imapd_userisadmin || index_hasrights(imapd_index, ACL_LOOKUP)) ?
          IMAP_PERMISSION_DENIED : IMAP_MAILBOX_NONEXISTENT;
    }

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        if (init.vanishedlist) seqset_free(init.vanishedlist);
        init.vanishedlist = NULL;
        if (doclose) index_close(&imapd_index);
        free(intname);
        return;
    }

    if (index_hasrights(imapd_index, ACL_EXPUNGE))
        warn_about_quota(imapd_index->mailbox->quotaroot);

    index_select(imapd_index, &init);

    if (init.vanishedlist) seqset_free(init.vanishedlist);
    init.vanishedlist = NULL;

    prot_printf(imapd_out, "%s OK [READ-%s] %s\r\n", tag,
                index_hasrights(imapd_index, ACL_READ_WRITE) ?
                "WRITE" : "ONLY", error_message(IMAP_OK_COMPLETED));

    syslog(LOG_DEBUG, "open: user %s opened %s", imapd_userid, name);
    free(intname);
    return;

 badlist:
    prot_printf(imapd_out, "%s BAD Invalid modifier list in %s\r\n", tag, cmd);
    eatline(imapd_in, c);
    return;

 badqresync:
    prot_printf(imapd_out, "%s BAD Invalid QRESYNC parameter list in %s\r\n",
                tag, cmd);
    eatline(imapd_in, c);
    return;
}
