static void cmd_id(char *tag)
{
    int c = EOF, npair = 0;
    static struct buf arg, field;

    /* check if we've already had an ID in non-authenticated state */
    if (!imapd_userid && imapd_id.did_id) {
        prot_printf(imapd_out, "%s OK NIL\r\n", tag);
        eatline(imapd_in, c);
        return;
    }

    clear_id();

    /* ok, accept parameter list */
    c = getword(imapd_in, &arg);
    /* check for "NIL" or start of parameter list */
    if (strcasecmp(arg.s, "NIL") && c != '(') {
        prot_printf(imapd_out, "%s BAD Invalid parameter list in Id\r\n", tag);
        eatline(imapd_in, c);
        return;
    }

    /* parse parameter list */
    if (c == '(') {
        for (;;) {
            if (c == ')') {
                /* end of string/value pairs */
                break;
            }

            /* get field name */
            c = getstring(imapd_in, imapd_out, &field);
            if (c != ' ') {
                prot_printf(imapd_out,
                            "%s BAD Invalid/missing field name in Id\r\n",
                            tag);
                eatline(imapd_in, c);
                return;
            }

            /* get field value */
            c = getnstring(imapd_in, imapd_out, &arg);
            if (c != ' ' && c != ')') {
                prot_printf(imapd_out,
                            "%s BAD Invalid/missing value in Id\r\n",
                            tag);
                eatline(imapd_in, c);
                return;
            }

            /* ok, we're anal, but we'll still process the ID command */
            if (strlen(field.s) > MAXIDFIELDLEN) {
                prot_printf(imapd_out,
                            "%s BAD field longer than %u octets in Id\r\n",
                            tag, MAXIDFIELDLEN);
                eatline(imapd_in, c);
                return;
            }
            if (arg.len > MAXIDVALUELEN) {
                prot_printf(imapd_out,
                            "%s BAD value longer than %u octets in Id\r\n",
                            tag, MAXIDVALUELEN);
                eatline(imapd_in, c);
                return;
            }
            if (++npair > MAXIDPAIRS) {
                prot_printf(imapd_out,
                            "%s BAD too many (%u) field-value pairs in ID\r\n",
                            tag, MAXIDPAIRS);
                eatline(imapd_in, c);
                return;
            }

            if (!strcmp(field.s, "os") && !strcmp(arg.s, "iOS")) {
                imapd_id.quirks |= QUIRK_SEARCHFUZZY;
            }

            /* ok, we're happy enough */
            appendattvalue(&imapd_id.params, field.s, &arg);
        }

        if (c != ')') {
            /* erp! */
            prot_printf(imapd_out, "%s BAD trailing junk\r\n", tag);
            eatline(imapd_in, c);
            return;
        }
        c = prot_getc(imapd_in);
    }

    /* check for CRLF */
    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out, "%s BAD Unexpected extra arguments to Id\r\n", tag);
        eatline(imapd_in, c);
        return;
    }

    /* log the client's ID string.
       eventually this should be a callback or something. */
    if (npair) {
        struct buf logbuf = BUF_INITIALIZER;
        struct attvaluelist *pptr;

        for (pptr = imapd_id.params; pptr; pptr = pptr->next) {
            const char *val = buf_cstring(&pptr->value);

            /* should we check for and format literals here ??? */
            buf_printf(&logbuf, " \"%s\" ", pptr->attrib);
            if (!val || !strcmp(val, "NIL"))
                buf_printf(&logbuf, "NIL");
            else
                buf_printf(&logbuf, "\"%s\"", val);
        }

        syslog(LOG_INFO, "client id sessionid=<%s>:%s", session_id(), buf_cstring(&logbuf));
        buf_free(&logbuf);
    }

    /* spit out our ID string.
       eventually this might be configurable. */
    if (config_getswitch(IMAPOPT_IMAPIDRESPONSE) &&
        (imapd_authstate || (config_serverinfo == IMAP_ENUM_SERVERINFO_ON))) {
        id_response(imapd_out);
        prot_printf(imapd_out, ")\r\n");
    }
    else
        prot_printf(imapd_out, "* ID NIL\r\n");

    imapd_check(NULL, 0);

    prot_printf(imapd_out, "%s OK %s\r\n", tag,
                error_message(IMAP_OK_COMPLETED));

    imapd_id.did_id = 1;
}
