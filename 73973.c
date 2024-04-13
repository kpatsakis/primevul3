static void cmd_changesub(char *tag, char *namespace, char *name, int add)
{
    const char *cmd = add ? "Subscribe" : "Unsubscribe";
    int r = 0;
    int force = config_getswitch(IMAPOPT_ALLOWALLSUBSCRIBE);

    if (backend_inbox || (backend_inbox = proxy_findinboxserver(imapd_userid))) {
        /* remote INBOX */
        if (add) {
            char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);
            r = mlookup(NULL, NULL, intname, NULL);
            free(intname);

            /* Doesn't exist on murder */
        }

        imapd_check(backend_inbox, 0);

        if (!r) {
            if (namespace) {
                prot_printf(backend_inbox->out,
                            "%s %s {" SIZE_T_FMT "+}\r\n%s"
                            " {" SIZE_T_FMT "+}\r\n%s\r\n",
                            tag, cmd,
                            strlen(namespace), namespace,
                            strlen(name), name);
            } else {
                prot_printf(backend_inbox->out, "%s %s {" SIZE_T_FMT "+}\r\n%s\r\n",
                            tag, cmd,
                            strlen(name), name);
            }
            pipe_including_tag(backend_inbox, tag, 0);
        }
        else {
            prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        }

        return;
    }

    /* local INBOX */
    if (namespace) lcase(namespace);
    if (!namespace || !strcmp(namespace, "mailbox")) {
        size_t len = strlen(name);
        if (force && imapd_namespace.isalt &&
            (((len == strlen(imapd_namespace.prefix[NAMESPACE_USER]) - 1) &&
              !strncmp(name, imapd_namespace.prefix[NAMESPACE_USER], len)) ||
             ((len == strlen(imapd_namespace.prefix[NAMESPACE_SHARED]) - 1) &&
              !strncmp(name, imapd_namespace.prefix[NAMESPACE_SHARED], len)))) {
            r = 0;
        }
        else {
            char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);
            r = mboxlist_changesub(intname, imapd_userid, imapd_authstate, add, force, 1);
            free(intname);
        }
    }
    else if (!strcmp(namespace, "bboard")) {
        r = add ? IMAP_MAILBOX_NONEXISTENT : 0;
    }
    else {
        prot_printf(imapd_out, "%s BAD Invalid %s subcommand\r\n", tag, cmd);
        return;
    }

    imapd_check(NULL, 0);

    if (r) {
        prot_printf(imapd_out, "%s NO %s: %s\r\n", tag, cmd, error_message(r));
    }
    else {
        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
    }
}
