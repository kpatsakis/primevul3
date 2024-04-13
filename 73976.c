static void cmd_create(char *tag, char *name, struct dlist *extargs, int localonly)
{
    int r = 0;
    int mbtype = 0;
    const char *partition = NULL;
    const char *server = NULL;
    struct buf specialuse = BUF_INITIALIZER;
    struct dlist *use;

    /* We don't care about trailing hierarchy delimiters. */
    if (name[0] && name[strlen(name)-1] == imapd_namespace.hier_sep) {
        name[strlen(name)-1] = '\0';
    }

    mbname_t *mbname = mbname_from_extname(name, &imapd_namespace, imapd_userid);

    dlist_getatom(extargs, "PARTITION", &partition);
    dlist_getatom(extargs, "SERVER", &server);

    const char *type = NULL;

    dlist_getatom(extargs, "PARTITION", &partition);
    dlist_getatom(extargs, "SERVER", &server);
    if (dlist_getatom(extargs, "TYPE", &type)) {
        if (!strcasecmp(type, "CALENDAR")) mbtype |= MBTYPE_CALENDAR;
        else if (!strcasecmp(type, "COLLECTION")) mbtype |= MBTYPE_COLLECTION;
        else if (!strcasecmp(type, "ADDRESSBOOK")) mbtype |= MBTYPE_ADDRESSBOOK;
        else {
            r = IMAP_MAILBOX_BADTYPE;
            goto err;
        }
    }
    use = dlist_getchild(extargs, "USE");
    if (use) {
        /* only user mailboxes can have specialuse, and they must be user toplevel folders */
        if (!mbname_userid(mbname) || strarray_size(mbname_boxes(mbname)) != 1) {
            r = IMAP_MAILBOX_SPECIALUSE;
            goto err;
        }
        /* I would much prefer to create the specialuse annotation FIRST
         * and do the sanity check on the values, so we can return the
         * correct error.  Sadly, that's a pain - so we compromise by
         * "normalising" first */
        struct dlist *item;
        char *raw;
        strarray_t *su = strarray_new();
        for (item = use->head; item; item = item->next) {
            strarray_append(su, dlist_cstring(item));
        }
        raw = strarray_join(su, " ");
        strarray_free(su);
        r = specialuse_validate(imapd_userid, raw, &specialuse);
        free(raw);
        if (r) {
            prot_printf(imapd_out, "%s NO [USEATTR] %s\r\n", tag, error_message(r));
            goto done;
        }
    }

    if ((server || partition) && !imapd_userisadmin) {
        if (config_mupdate_config == IMAP_ENUM_MUPDATE_CONFIG_STANDARD ||
            config_mupdate_config == IMAP_ENUM_MUPDATE_CONFIG_UNIFIED) {

            if (!config_getstring(IMAPOPT_PROXYSERVERS)) {
                r = IMAP_PERMISSION_DENIED;
                goto err;
            }
        }
    }

    /* check for INBOX.INBOX creation by broken Apple clients */
    const strarray_t *boxes = mbname_boxes(mbname);
    if (strarray_size(boxes) > 1
        && !strcasecmp(strarray_nth(boxes, 0), "INBOX")
        && !strcasecmp(strarray_nth(boxes, 1), "INBOX"))
        r = IMAP_MAILBOX_BADNAME;

    if (r) {
    err:
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        goto done;
    }

    if (!localonly) {

        if (config_mupdate_server) {

            if (
                    config_mupdate_config == IMAP_ENUM_MUPDATE_CONFIG_STANDARD ||
                    config_mupdate_config == IMAP_ENUM_MUPDATE_CONFIG_UNIFIED
                ) {

                if (!config_getstring(IMAPOPT_PROXYSERVERS)) {
                    mbentry_t *parent = NULL;

                    r = mboxlist_findparent(mbname_intname(mbname), &parent);

                    if (r) {
                        if (r != IMAP_MAILBOX_NONEXISTENT) {
                            prot_printf(imapd_out, "%s NO %s (%s:%d)\r\n", tag, error_message(r), __FILE__, __LINE__);
                            goto done;
                        }
                    }

                    if (!server && !partition) {
                        if (!parent) {
                            server = find_free_server();

                            if (!server) {
                                prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(IMAP_SERVER_UNAVAILABLE));
                                goto done;
                            }

                        } else {
                            server = parent->server;
                            /* DO NOT set the partition:
                               only admins are allowed to do this
                               and the backend will use the partition
                               of the parent by default anyways.

                            partition = parent->partition;
                            */
                        }
                    }

                    struct backend *s_conn = NULL;

                    s_conn = proxy_findserver(
                            server,
                            &imap_protocol,
                            proxy_userid,
                            &backend_cached,
                            &backend_current,
                            &backend_inbox,
                            imapd_in
                        );

                    if (!s_conn) {
                        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(IMAP_SERVER_UNAVAILABLE));
                        goto done;
                    }

                    if (imapd_userisadmin && supports_referrals) {
                        imapd_refer(tag, server, name);
                        referral_kick = 1;
                        return;
                    }

                    if (!CAPA(s_conn, CAPA_MUPDATE)) {
                        syslog(LOG_WARNING, "backend %s is not advertising any MUPDATE capability (%s:%d)", server, __FILE__, __LINE__);
                    }

                    prot_printf(s_conn->out, "%s CREATE ", tag);
                    prot_printastring(s_conn->out, name);

                    if (specialuse.len) {
                        prot_printf(s_conn->out, "(USE (%s)", buf_cstring(&specialuse));

                        if (partition) {
                            prot_printf(s_conn->out, " PARTITION ");
                            prot_printastring(s_conn->out, partition);
                        }

                        prot_putc(')', s_conn->out);
                    }

                    else if (partition) {
                        prot_putc(' ', s_conn->out);
                        prot_printastring(s_conn->out, partition);
                    }

                    prot_printf(s_conn->out, "\r\n");

                    int res = pipe_until_tag(s_conn, tag, 0);

                    if (!CAPA(s_conn, CAPA_MUPDATE)) {
                        syslog(LOG_WARNING, "backend %s is not advertising any MUPDATE capability (%s:%d)", server, __FILE__, __LINE__);
                    }

                    /* make sure we've seen the update */
                    if (ultraparanoid && res == PROXY_OK) kick_mupdate();

                    imapd_check(s_conn, 0);

                    prot_printf(imapd_out, "%s %s", tag, s_conn->last_result.s);

                    goto done;

                } else { // (!config_getstring(IMAPOPT_PROXYSERVERS))
                    goto localcreate;

                } // (!config_getstring(IMAPOPT_PROXYSERVERS))

            } // (config_mupdate_config == IMAP_ENUM_MUPDATE_CONFIG_STANDARD)

            else if (config_mupdate_config == IMAP_ENUM_MUPDATE_CONFIG_REPLICATED) {
                goto localcreate;
            } // (config_mupdate_config == IMAP_ENUM_MUPDATE_CONFIG_REPLICATED)

            else {
                syslog(LOG_ERR, "murder configuration I cannot deal with");
                prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(IMAP_SERVER_UNAVAILABLE));
                goto done;
            }

        } else { // (config_mupdate_server)
            goto localcreate;
        } // (config_mupdate_server)

    } else { // (!localonly)
        goto localcreate;
    }

localcreate:
    r = mboxlist_createmailbox(
            mbname_intname(mbname),                             // const char name
            mbtype,                                             // int mbtype
            partition,                                          // const char partition
            imapd_userisadmin || imapd_userisproxyadmin,        // int isadmin
            imapd_userid,                                       // const char userid
            imapd_authstate,                                    // struct auth_state auth_state
            localonly,                                          // int localonly
            localonly,                                          // int forceuser
            0,                                                  // int dbonly
            1,                                                  // int notify
            NULL                                                // struct mailbox mailboxptr
        );

#ifdef USE_AUTOCREATE
    if (r == IMAP_PERMISSION_DENIED) {
        if (!strcasecmp(name, "INBOX")) {
            int autocreatequotastorage = config_getint(IMAPOPT_AUTOCREATE_QUOTA);

            if (autocreatequotastorage > 0) {
                r = mboxlist_createmailbox(
                        mbname_intname(mbname),
                        0,
                        partition,
                        1,
                        imapd_userid,
                        imapd_authstate,
                        0,
                        0,
                        0,
                        1,
                        NULL
                    );

                if (r) {
                    prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
                    goto done;
                }

                int autocreatequotamessage = config_getint(IMAPOPT_AUTOCREATE_QUOTA_MESSAGES);

                if ((autocreatequotastorage > 0) || (autocreatequotamessage > 0)) {
                    quota_t newquotas[QUOTA_NUMRESOURCES];
                    int res;

                    for (res = 0; res < QUOTA_NUMRESOURCES; res++) {
                        newquotas[res] = QUOTA_UNLIMITED;
                    }

                    newquotas[QUOTA_STORAGE] = autocreatequotastorage;
                    newquotas[QUOTA_MESSAGE] = autocreatequotamessage;

                    (void) mboxlist_setquotas(mbname_intname(mbname), newquotas, 0);
                } // (autocreatequotastorage > 0) || (autocreatequotamessage > 0)

            } else { // (autocreatequotastorage = config_getint(IMAPOPT_AUTOCREATEQUOTA))
                prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(IMAP_PERMISSION_DENIED));
                goto done;

            } // (autocreatequotastorage = config_getint(IMAPOPT_AUTOCREATEQUOTA))

        } else { // (!strcasecmp(name, "INBOX"))
            prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(IMAP_PERMISSION_DENIED));
            goto done;
        } // (!strcasecmp(name, "INBOX"))

    } else if (r) { // (r == IMAP_PERMISSION_DENIED)
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        goto done;

    } else { // (r == IMAP_PERMISSION_DENIED)
        /* no error: carry on */
    } // (r == IMAP_PERMISSION_DENIED)

#else // USE_AUTOCREATE
    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        goto done;

    } // (r)
#endif // USE_AUTOCREATE

    if (specialuse.len) {
        r = annotatemore_write(mbname_intname(mbname), "/specialuse", mbname_userid(mbname), &specialuse);
        if (r) {
            /* XXX - failure here SHOULD cause a cleanup of the created mailbox */
            syslog(
                    LOG_ERR,
                    "IOERROR: failed to write specialuse for %s on %s (%s) (%s:%d)",
                    imapd_userid,
                    mbname_intname(mbname),
                    buf_cstring(&specialuse),
                    __FILE__,
                    __LINE__
                );

            prot_printf(imapd_out, "%s NO %s (%s:%d)\r\n", tag, error_message(r), __FILE__, __LINE__);
            goto done;
        }
    }

    prot_printf(imapd_out, "%s OK Completed\r\n", tag);

    imapd_check(NULL, 0);

done:
    buf_free(&specialuse);
    mbname_free(&mbname);
}
