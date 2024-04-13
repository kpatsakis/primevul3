static void cmd_store(char *tag, char *sequence, int usinguid)
{
    const char *cmd = usinguid ? "UID Store" : "Store";
    struct storeargs storeargs;
    static struct buf operation, flagname;
    int len, c;
    int flagsparsed = 0, inlist = 0;
    char *modified = NULL;
    int r;

    if (backend_current) {
        /* remote mailbox */
        prot_printf(backend_current->out, "%s %s %s ",
                    tag, cmd, sequence);
        if (!pipe_command(backend_current, 65536)) {
            pipe_including_tag(backend_current, tag, 0);
        }
        return;
    }

    /* local mailbox */
    memset(&storeargs, 0, sizeof storeargs);
    storeargs.unchangedsince = ~0ULL;
    storeargs.usinguid = usinguid;
    strarray_init(&storeargs.flags);

    c = prot_getc(imapd_in);
    if (c == '(') {
        /* Grab/parse the modifier(s) */
        static struct buf storemod;

        do {
            c = getword(imapd_in, &storemod);
            ucase(storemod.s);
            if (!strcmp(storemod.s, "UNCHANGEDSINCE")) {
                if (c != ' ') {
                    prot_printf(imapd_out,
                                "%s BAD Missing required argument to %s %s\r\n",
                                tag, cmd, storemod.s);
                    eatline(imapd_in, c);
                    return;
                }
                c = getmodseq(imapd_in, &storeargs.unchangedsince);
                if (c == EOF) {
                    prot_printf(imapd_out,
                                "%s BAD Invalid argument to %s UNCHANGEDSINCE\r\n",
                                tag, cmd);
                    eatline(imapd_in, c);
                    return;
                }
            }
            else {
                prot_printf(imapd_out, "%s BAD Invalid %s modifier %s\r\n",
                            tag, cmd, storemod.s);
                eatline(imapd_in, c);
                return;
            }
        } while (c == ' ');
        if (c != ')') {
            prot_printf(imapd_out,
                        "%s BAD Missing close paren in store modifier entry \r\n",
                        tag);
            eatline(imapd_in, c);
            return;
        }
        c = prot_getc(imapd_in);
        if (c != ' ') {
            prot_printf(imapd_out,
                        "%s BAD Missing required argument to %s\r\n",
                        tag, cmd);
            eatline(imapd_in, c);
            return;
        }
    }
    else
        prot_ungetc(c, imapd_in);

    c = getword(imapd_in, &operation);
    if (c != ' ') {
        prot_printf(imapd_out,
                    "%s BAD Missing required argument to %s\r\n", tag, cmd);
        eatline(imapd_in, c);
        return;
    }
    lcase(operation.s);

    len = strlen(operation.s);
    if (len > 7 && !strcmp(operation.s+len-7, ".silent")) {
        storeargs.silent = 1;
        operation.s[len-7] = '\0';
    }

    if (!strcmp(operation.s, "+flags")) {
        storeargs.operation = STORE_ADD_FLAGS;
    }
    else if (!strcmp(operation.s, "-flags")) {
        storeargs.operation = STORE_REMOVE_FLAGS;
    }
    else if (!strcmp(operation.s, "flags")) {
        storeargs.operation = STORE_REPLACE_FLAGS;
    }
    else if (!strcmp(operation.s, "annotation")) {
        storeargs.operation = STORE_ANNOTATION;
        /* ANNOTATION has implicit .SILENT behaviour */
        storeargs.silent = 1;

        c = parse_annotate_store_data(tag, /*permessage_flag*/1,
                                      &storeargs.entryatts);
        if (c == EOF) {
            eatline(imapd_in, c);
            goto freeflags;
        }
        storeargs.namespace = &imapd_namespace;
        storeargs.isadmin = imapd_userisadmin;
        storeargs.userid = imapd_userid;
        storeargs.authstate = imapd_authstate;
        goto notflagsdammit;
    }
    else {
        prot_printf(imapd_out, "%s BAD Invalid %s attribute\r\n", tag, cmd);
        eatline(imapd_in, ' ');
        return;
    }

    for (;;) {
        c = getword(imapd_in, &flagname);
        if (c == '(' && !flagname.s[0] && !flagsparsed && !inlist) {
            inlist = 1;
            continue;
        }

        if (!flagname.s[0]) break;

        if (flagname.s[0] == '\\') {
            lcase(flagname.s);
            if (!strcmp(flagname.s, "\\seen")) {
                storeargs.seen = 1;
            }
            else if (!strcmp(flagname.s, "\\answered")) {
                storeargs.system_flags |= FLAG_ANSWERED;
            }
            else if (!strcmp(flagname.s, "\\flagged")) {
                storeargs.system_flags |= FLAG_FLAGGED;
            }
            else if (!strcmp(flagname.s, "\\deleted")) {
                storeargs.system_flags |= FLAG_DELETED;
            }
            else if (!strcmp(flagname.s, "\\draft")) {
                storeargs.system_flags |= FLAG_DRAFT;
            }
            else {
                prot_printf(imapd_out, "%s BAD Invalid system flag in %s command\r\n",
                       tag, cmd);
                eatline(imapd_in, c);
                goto freeflags;
            }
        }
        else if (!imparse_isatom(flagname.s)) {
            prot_printf(imapd_out, "%s BAD Invalid flag name %s in %s command\r\n",
                   tag, flagname.s, cmd);
            eatline(imapd_in, c);
            goto freeflags;
        }
        else
            strarray_append(&storeargs.flags, flagname.s);

        flagsparsed++;
        if (c != ' ') break;
    }

    if (!inlist && !flagsparsed) {
        prot_printf(imapd_out, "%s BAD Missing required argument to %s\r\n", tag, cmd);
        eatline(imapd_in, c);
        return;
    }
    if (inlist && c == ')') {
        inlist = 0;
        c = prot_getc(imapd_in);
    }
    if (inlist) {
        prot_printf(imapd_out, "%s BAD Missing close parenthesis in %s\r\n", tag, cmd);
        eatline(imapd_in, c);
        goto freeflags;
    }
notflagsdammit:
    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out, "%s BAD Unexpected extra arguments to %s\r\n", tag, cmd);
        eatline(imapd_in, c);
        goto freeflags;
    }

    if ((storeargs.unchangedsince != ULONG_MAX) &&
        !(client_capa & CAPA_CONDSTORE)) {
        client_capa |= CAPA_CONDSTORE;
        prot_printf(imapd_out, "* OK [HIGHESTMODSEQ " MODSEQ_FMT "]  \r\n",
                    index_highestmodseq(imapd_index));
    }

    r = index_store(imapd_index, sequence, &storeargs);

    /* format the MODIFIED response code */
    if (storeargs.modified) {
        char *seqstr = seqset_cstring(storeargs.modified);
        assert(seqstr);
        modified = strconcat("[MODIFIED ", seqstr, "] ", (char *)NULL);
        free(seqstr);
    }
    else {
        modified = xstrdup("");
    }

    if (r) {
        prot_printf(imapd_out, "%s NO %s%s\r\n",
                    tag, modified, error_message(r));
    }
    else {
        prot_printf(imapd_out, "%s OK %s%s\r\n",
                    tag, modified, error_message(IMAP_OK_COMPLETED));
    }

 freeflags:
    strarray_fini(&storeargs.flags);
    freeentryatts(storeargs.entryatts);
    seqset_free(storeargs.modified);
    free(modified);
}
