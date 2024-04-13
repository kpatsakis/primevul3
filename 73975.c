static void cmd_copy(char *tag, char *sequence, char *name, int usinguid, int ismove)
{
    int r, myrights;
    char *copyuid = NULL;
    mbentry_t *mbentry = NULL;

    char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);
    r = mlookup(NULL, NULL, intname, &mbentry);

    if (!r) myrights = cyrus_acl_myrights(imapd_authstate, mbentry->acl);

    if (!r && backend_current) {
        /* remote mailbox -> local or remote mailbox */

        /* xxx  start of separate proxy-only code
           (remove when we move to a unified environment) */
        struct backend *s = NULL;

        s = proxy_findserver(mbentry->server, &imap_protocol,
                             proxy_userid, &backend_cached,
                             &backend_current, &backend_inbox, imapd_in);
        mboxlist_entry_free(&mbentry);
        if (!s) {
            r = IMAP_SERVER_UNAVAILABLE;
            goto done;
        }

        if (s != backend_current) {
            /* this is the hard case; we have to fetch the messages and append
               them to the other mailbox */

            proxy_copy(tag, sequence, name, myrights, usinguid, s);
            goto cleanup;
        }
        /* xxx  end of separate proxy-only code */

        /* simply send the COPY to the backend */
        prot_printf(
                backend_current->out,
                "%s %s %s {" SIZE_T_FMT "+}\r\n%s\r\n",
                tag,
                usinguid ? (ismove ? "UID Move" : "UID Copy") : (ismove ? "Move" : "Copy"),
                sequence,
                strlen(name),
                name
            );

        pipe_including_tag(backend_current, tag, 0);

        goto cleanup;
    }
    else if (!r && (mbentry->mbtype & MBTYPE_REMOTE)) {
        /* local mailbox -> remote mailbox
         *
         * fetch the messages and APPEND them to the backend
         *
         * xxx  completely untested
         */
        struct backend *s = NULL;
        int res;

        s = proxy_findserver(mbentry->server, &imap_protocol,
                             proxy_userid, &backend_cached,
                             &backend_current, &backend_inbox, imapd_in);
        mboxlist_entry_free(&mbentry);
        if (!s) r = IMAP_SERVER_UNAVAILABLE;
        else if (!CAPA(s, CAPA_MULTIAPPEND)) {
            /* we need MULTIAPPEND for atomicity */
            r = IMAP_REMOTE_NO_MULTIAPPEND;
        }

        if (r) goto done;

        assert(!ismove); /* XXX - support proxying moves */

        /* start the append */
        prot_printf(s->out, "%s Append {" SIZE_T_FMT "+}\r\n%s",
                    tag, strlen(name), name);

        /* append the messages */
        r = index_copy_remote(imapd_index, sequence, usinguid, s->out);

        if (!r) {
            /* ok, finish the append; we need the UIDVALIDITY and UIDs
               to return as part of our COPYUID response code */
            char *appenduid, *b;

            prot_printf(s->out, "\r\n");

            res = pipe_until_tag(s, tag, 0);

            if (res == PROXY_OK) {
                if (myrights & ACL_READ) {
                    appenduid = strchr(s->last_result.s, '[');
                    /* skip over APPENDUID */
                    if (appenduid) {
                        appenduid += strlen("[appenduid ");
                        b = strchr(appenduid, ']');
                        if (b) *b = '\0';
                        prot_printf(imapd_out, "%s OK [COPYUID %s] %s\r\n", tag,
                                    appenduid, error_message(IMAP_OK_COMPLETED));
                    } else
                        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                                    error_message(IMAP_OK_COMPLETED));
                } else {
                    prot_printf(imapd_out, "%s OK %s\r\n", tag,
                                error_message(IMAP_OK_COMPLETED));
                }
            } else {
                prot_printf(imapd_out, "%s %s", tag, s->last_result.s);
            }
        } else {
            /* abort the append */
            prot_printf(s->out, " {0}\r\n");
            pipe_until_tag(s, tag, 0);

            /* report failure */
            prot_printf(imapd_out, "%s NO inter-server COPY failed\r\n", tag);
        }

        goto cleanup;
    }

    /* need permission to delete from source if it's a move */
    if (!r && ismove && !(imapd_index->myrights & ACL_EXPUNGE))
        r = IMAP_PERMISSION_DENIED;

    /* local mailbox -> local mailbox */
    if (!r) {
        r = index_copy(imapd_index, sequence, usinguid, intname,
                       &copyuid, !config_getswitch(IMAPOPT_SINGLEINSTANCESTORE),
                       &imapd_namespace,
                       (imapd_userisadmin || imapd_userisproxyadmin), ismove,
                       ignorequota);
    }

    if (ismove && copyuid && !r) {
        prot_printf(imapd_out, "* OK [COPYUID %s] %s\r\n",
                    copyuid, error_message(IMAP_OK_COMPLETED));
        free(copyuid);
        copyuid = NULL;
    }

    imapd_check(NULL, ismove || usinguid);

  done:

    if (r && !(usinguid && r == IMAP_NO_NOSUCHMSG)) {
        prot_printf(imapd_out, "%s NO %s%s\r\n", tag,
                    (r == IMAP_MAILBOX_NONEXISTENT &&
                     mboxlist_createmailboxcheck(intname, 0, 0,
                                                 imapd_userisadmin,
                                                 imapd_userid, imapd_authstate,
                                                 NULL, NULL, 0) == 0)
                    ? "[TRYCREATE] " : "", error_message(r));
    }
    else if (copyuid) {
            prot_printf(imapd_out, "%s OK [COPYUID %s] %s\r\n", tag,
                        copyuid, error_message(IMAP_OK_COMPLETED));
            free(copyuid);
    }
    else {
        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
    }

cleanup:
    mboxlist_entry_free(&mbentry);
    free(intname);
}
