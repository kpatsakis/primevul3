static int parse_fetch_args(const char *tag, const char *cmd,
                            int allow_vanished,
                            struct fetchargs *fa)
{
    static struct buf fetchatt, fieldname;
    int c;
    int inlist = 0;
    char *p, *section;
    struct octetinfo oi;
    strarray_t *newfields = strarray_new();

    c = getword(imapd_in, &fetchatt);
    if (c == '(' && !fetchatt.s[0]) {
        inlist = 1;
        c = getword(imapd_in, &fetchatt);
    }
    for (;;) {
        ucase(fetchatt.s);
        switch (fetchatt.s[0]) {
        case 'A':
            if (!inlist && !strcmp(fetchatt.s, "ALL")) {
                fa->fetchitems |= FETCH_ALL;
            }
            else if (!strcmp(fetchatt.s, "ANNOTATION")) {
                fa->fetchitems |= FETCH_ANNOTATION;
                if (c != ' ')
                    goto badannotation;
                c = prot_getc(imapd_in);
                if (c != '(')
                    goto badannotation;
                c = parse_annotate_fetch_data(tag,
                                              /*permessage_flag*/1,
                                              &fa->entries,
                                              &fa->attribs);
                if (c == EOF) {
                    eatline(imapd_in, c);
                    goto freeargs;
                }
                if (c != ')') {
badannotation:
                    prot_printf(imapd_out, "%s BAD invalid Annotation\r\n", tag);
                    eatline(imapd_in, c);
                    goto freeargs;
                }
                c = prot_getc(imapd_in);
            }
            else goto badatt;
            break;

        case 'B':
            if (!strncmp(fetchatt.s, "BINARY[", 7) ||
                !strncmp(fetchatt.s, "BINARY.PEEK[", 12) ||
                !strncmp(fetchatt.s, "BINARY.SIZE[", 12)) {
                int binsize = 0;

                p = section = fetchatt.s + 7;
                if (!strncmp(p, "PEEK[", 5)) {
                    p = section += 5;
                }
                else if (!strncmp(p, "SIZE[", 5)) {
                    p = section += 5;
                    binsize = 1;
                }
                else {
                    fa->fetchitems |= FETCH_SETSEEN;
                }
                while (Uisdigit(*p) || *p == '.') {
                    if (*p == '.' && !Uisdigit(p[-1])) break;
                    /* Part number cannot begin with '0' */
                    if (*p == '0' && !Uisdigit(p[-1])) break;
                    p++;
                }

                if (*p != ']') {
                    prot_printf(imapd_out, "%s BAD Invalid binary section\r\n", tag);
                    eatline(imapd_in, c);
                    goto freeargs;
                }
                p++;

                if (!binsize) PARSE_PARTIAL(oi.start_octet, oi.octet_count);

                if (*p) {
                    prot_printf(imapd_out, "%s BAD Junk after binary section\r\n", tag);
                    eatline(imapd_in, c);
                    goto freeargs;
                }
                if (binsize)
                    section_list_append(&fa->sizesections, section, &oi);
                else
                    section_list_append(&fa->binsections, section, &oi);
            }
            else if (!strcmp(fetchatt.s, "BODY")) {
                fa->fetchitems |= FETCH_BODY;
            }
            else if (!strcmp(fetchatt.s, "BODYSTRUCTURE")) {
                fa->fetchitems |= FETCH_BODYSTRUCTURE;
            }
            else if (!strncmp(fetchatt.s, "BODY[", 5) ||
                     !strncmp(fetchatt.s, "BODY.PEEK[", 10)) {
                p = section = fetchatt.s + 5;
                if (!strncmp(p, "PEEK[", 5)) {
                    p = section += 5;
                }
                else {
                    fa->fetchitems |= FETCH_SETSEEN;
                }
                while (Uisdigit(*p) || *p == '.') {
                    if (*p == '.' && !Uisdigit(p[-1])) break;
                    /* Obsolete section 0 can only occur before close brace */
                    if (*p == '0' && !Uisdigit(p[-1]) && p[1] != ']') break;
                    p++;
                }

                if (*p == 'H' && !strncmp(p, "HEADER.FIELDS", 13) &&
                    (p == section || p[-1] == '.') &&
                    (p[13] == '\0' || !strcmp(p+13, ".NOT"))) {

                    /*
                     * If not top-level or a HEADER.FIELDS.NOT, can't pull
                     * the headers out of the cache.
                     */
                    if (p != section || p[13] != '\0') {
                        fa->cache_atleast = BIT32_MAX;
                    }

                    if (c != ' ') {
                        prot_printf(imapd_out,
                                    "%s BAD Missing required argument to %s %s\r\n",
                                    tag, cmd, fetchatt.s);
                        eatline(imapd_in, c);
                        goto freeargs;
                    }
                    c = prot_getc(imapd_in);
                    if (c != '(') {
                        prot_printf(imapd_out, "%s BAD Missing required open parenthesis in %s %s\r\n",
                                    tag, cmd, fetchatt.s);
                        eatline(imapd_in, c);
                        goto freeargs;
                    }
                    do {
                        c = getastring(imapd_in, imapd_out, &fieldname);
                        for (p = fieldname.s; *p; p++) {
                            if (*p <= ' ' || *p & 0x80 || *p == ':') break;
                        }
                        if (*p || !*fieldname.s) {
                            prot_printf(imapd_out, "%s BAD Invalid field-name in %s %s\r\n",
                                        tag, cmd, fetchatt.s);
                            eatline(imapd_in, c);
                            goto freeargs;
                        }
                        strarray_append(newfields, fieldname.s);
                        if (fa->cache_atleast < BIT32_MAX) {
                            bit32 this_ver =
                                mailbox_cached_header(fieldname.s);
                            if(this_ver > fa->cache_atleast)
                                fa->cache_atleast = this_ver;
                        }
                    } while (c == ' ');
                    if (c != ')') {
                        prot_printf(imapd_out, "%s BAD Missing required close parenthesis in %s %s\r\n",
                                    tag, cmd, fetchatt.s);
                        eatline(imapd_in, c);
                        goto freeargs;
                    }

                    /* Grab/parse the ]<x.y> part */
                    c = getword(imapd_in, &fieldname);
                    p = fieldname.s;
                    if (*p++ != ']') {
                        prot_printf(imapd_out, "%s BAD Missing required close bracket after %s %s\r\n",
                                    tag, cmd, fetchatt.s);
                        eatline(imapd_in, c);
                        goto freeargs;
                    }

                    PARSE_PARTIAL(oi.start_octet, oi.octet_count);

                    if (*p) {
                        prot_printf(imapd_out, "%s BAD Junk after body section\r\n", tag);
                        eatline(imapd_in, c);
                        goto freeargs;
                    }
                    appendfieldlist(&fa->fsections,
                                    section, newfields, fieldname.s,
                                    &oi, sizeof(oi));
                    /* old 'newfields' is managed by the fieldlist now */
                    newfields = strarray_new();
                    break;
                }

                switch (*p) {
                case 'H':
                    if (p != section && p[-1] != '.') break;
                    if (!strncmp(p, "HEADER]", 7)) p += 6;
                    break;

                case 'M':
                    if (!strncmp(p-1, ".MIME]", 6)) p += 4;
                    break;

                case 'T':
                    if (p != section && p[-1] != '.') break;
                    if (!strncmp(p, "TEXT]", 5)) p += 4;
                    break;
                }

                if (*p != ']') {
                    prot_printf(imapd_out, "%s BAD Invalid body section\r\n", tag);
                    eatline(imapd_in, c);
                    goto freeargs;
                }
                p++;

                PARSE_PARTIAL(oi.start_octet, oi.octet_count);

                if (*p) {
                    prot_printf(imapd_out, "%s BAD Junk after body section\r\n", tag);
                    eatline(imapd_in, c);
                    goto freeargs;
                }
                section_list_append(&fa->bodysections, section, &oi);
            }
            else goto badatt;
            break;

        case 'C':
            if (!strcmp(fetchatt.s, "CID") &&
                config_getswitch(IMAPOPT_CONVERSATIONS)) {
                fa->fetchitems |= FETCH_CID;
            }
            else goto badatt;
            break;

        case 'D':
            if (!strcmp(fetchatt.s, "DIGEST.SHA1")) {
                fa->fetchitems |= FETCH_GUID;
            }
            else goto badatt;
            break;

        case 'E':
            if (!strcmp(fetchatt.s, "ENVELOPE")) {
                fa->fetchitems |= FETCH_ENVELOPE;
            }
            else goto badatt;
            break;

        case 'F':
            if (!inlist && !strcmp(fetchatt.s, "FAST")) {
                fa->fetchitems |= FETCH_FAST;
            }
            else if (!inlist && !strcmp(fetchatt.s, "FULL")) {
                fa->fetchitems |= FETCH_FULL;
            }
            else if (!strcmp(fetchatt.s, "FLAGS")) {
                fa->fetchitems |= FETCH_FLAGS;
            }
            else if (!strcmp(fetchatt.s, "FOLDER")) {
                fa->fetchitems |= FETCH_FOLDER;
            }
            else goto badatt;
            break;

        case 'I':
            if (!strcmp(fetchatt.s, "INTERNALDATE")) {
                fa->fetchitems |= FETCH_INTERNALDATE;
            }
            else goto badatt;
            break;

        case 'M':
            if (!strcmp(fetchatt.s, "MODSEQ")) {
                fa->fetchitems |= FETCH_MODSEQ;
            }
            else goto badatt;
            break;

        case 'R':
            if (!strcmp(fetchatt.s, "RFC822")) {
                fa->fetchitems |= FETCH_RFC822|FETCH_SETSEEN;
            }
            else if (!strcmp(fetchatt.s, "RFC822.HEADER")) {
                fa->fetchitems |= FETCH_HEADER;
            }
            else if (!strcmp(fetchatt.s, "RFC822.PEEK")) {
                fa->fetchitems |= FETCH_RFC822;
            }
            else if (!strcmp(fetchatt.s, "RFC822.SIZE")) {
                fa->fetchitems |= FETCH_SIZE;
            }
            else if (!strcmp(fetchatt.s, "RFC822.TEXT")) {
                fa->fetchitems |= FETCH_TEXT|FETCH_SETSEEN;
            }
            else if (!strcmp(fetchatt.s, "RFC822.SHA1")) {
                fa->fetchitems |= FETCH_SHA1;
            }
            else if (!strcmp(fetchatt.s, "RFC822.FILESIZE")) {
                fa->fetchitems |= FETCH_FILESIZE;
            }
            else if (!strcmp(fetchatt.s, "RFC822.TEXT.PEEK")) {
                fa->fetchitems |= FETCH_TEXT;
            }
            else if (!strcmp(fetchatt.s, "RFC822.HEADER.LINES") ||
                     !strcmp(fetchatt.s, "RFC822.HEADER.LINES.NOT")) {
                if (c != ' ') {
                    prot_printf(imapd_out, "%s BAD Missing required argument to %s %s\r\n",
                           tag, cmd, fetchatt.s);
                    eatline(imapd_in, c);
                    goto freeargs;
                }
                c = prot_getc(imapd_in);
                if (c != '(') {
                    prot_printf(imapd_out, "%s BAD Missing required open parenthesis in %s %s\r\n",
                           tag, cmd, fetchatt.s);
                    eatline(imapd_in, c);
                    goto freeargs;
                }
                do {
                    c = getastring(imapd_in, imapd_out, &fieldname);
                    for (p = fieldname.s; *p; p++) {
                        if (*p <= ' ' || *p & 0x80 || *p == ':') break;
                    }
                    if (*p || !*fieldname.s) {
                        prot_printf(imapd_out, "%s BAD Invalid field-name in %s %s\r\n",
                               tag, cmd, fetchatt.s);
                        eatline(imapd_in, c);
                        goto freeargs;
                    }
                    lcase(fieldname.s);;
                    /* 19 is magic number -- length of
                     * "RFC822.HEADERS.NOT" */
                    strarray_append(strlen(fetchatt.s) == 19 ?
                                  &fa->headers : &fa->headers_not,
                                  fieldname.s);
                    if (strlen(fetchatt.s) != 19) {
                        fa->cache_atleast = BIT32_MAX;
                    }
                    if (fa->cache_atleast < BIT32_MAX) {
                        bit32 this_ver =
                            mailbox_cached_header(fieldname.s);
                        if(this_ver > fa->cache_atleast)
                            fa->cache_atleast = this_ver;
                   }
                } while (c == ' ');
                if (c != ')') {
                    prot_printf(imapd_out, "%s BAD Missing required close parenthesis in %s %s\r\n",
                           tag, cmd, fetchatt.s);
                    eatline(imapd_in, c);
                    goto freeargs;
                }
                c = prot_getc(imapd_in);
            }
            else goto badatt;
            break;

        case 'U':
            if (!strcmp(fetchatt.s, "UID")) {
                fa->fetchitems |= FETCH_UID;
            }
            else if (!strcmp(fetchatt.s, "UIDVALIDITY")) {
                fa->fetchitems |= FETCH_UIDVALIDITY;
            }
            else goto badatt;
            break;

        default:
        badatt:
            prot_printf(imapd_out, "%s BAD Invalid %s attribute %s\r\n", tag, cmd, fetchatt.s);
            eatline(imapd_in, c);
            goto freeargs;
        }

        if (inlist && c == ' ') c = getword(imapd_in, &fetchatt);
        else break;
    }

    if (inlist && c == ')') {
        inlist = 0;
        c = prot_getc(imapd_in);
    }
    if (inlist) {
        prot_printf(imapd_out, "%s BAD Missing close parenthesis in %s\r\n",
                    tag, cmd);
        eatline(imapd_in, c);
        goto freeargs;
    }

    if (c == ' ') {
        /* Grab/parse the modifier(s) */
        c = prot_getc(imapd_in);
        if (c != '(') {
            prot_printf(imapd_out,
                        "%s BAD Missing required open parenthesis in %s modifiers\r\n",
                        tag, cmd);
            eatline(imapd_in, c);
            goto freeargs;
        }
        do {
            c = getword(imapd_in, &fetchatt);
            ucase(fetchatt.s);
            if (!strcmp(fetchatt.s, "CHANGEDSINCE")) {
                if (c != ' ') {
                    prot_printf(imapd_out,
                                "%s BAD Missing required argument to %s %s\r\n",
                                tag, cmd, fetchatt.s);
                    eatline(imapd_in, c);
                    goto freeargs;
                }
                c = getmodseq(imapd_in, &fa->changedsince);
                if (c == EOF) {
                    prot_printf(imapd_out,
                                "%s BAD Invalid argument to %s %s\r\n",
                                tag, cmd, fetchatt.s);
                    eatline(imapd_in, c);
                    goto freeargs;
                }
                fa->fetchitems |= FETCH_MODSEQ;
            }
            else if (allow_vanished &&
                     !strcmp(fetchatt.s, "VANISHED")) {
                fa->vanished = 1;
            }
            else {
                prot_printf(imapd_out, "%s BAD Invalid %s modifier %s\r\n",
                            tag, cmd, fetchatt.s);
                eatline(imapd_in, c);
                goto freeargs;
            }
        } while (c == ' ');
        if (c != ')') {
            prot_printf(imapd_out, "%s BAD Missing close parenthesis in %s\r\n",
                        tag, cmd);
            eatline(imapd_in, c);
            goto freeargs;
        }
        c = prot_getc(imapd_in);
    }

    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out, "%s BAD Unexpected extra arguments to %s\r\n", tag, cmd);
        eatline(imapd_in, c);
        goto freeargs;
    }

    if (!fa->fetchitems && !fa->bodysections && !fa->fsections &&
        !fa->binsections && !fa->sizesections &&
        !fa->headers.count && !fa->headers_not.count) {
        prot_printf(imapd_out, "%s BAD Missing required argument to %s\r\n", tag, cmd);
        goto freeargs;
    }

    if (fa->vanished && !fa->changedsince) {
        prot_printf(imapd_out, "%s BAD Missing required argument to %s\r\n", tag, cmd);
        goto freeargs;
    }

    if (fa->fetchitems & FETCH_MODSEQ) {
        if (!(client_capa & CAPA_CONDSTORE)) {
            client_capa |= CAPA_CONDSTORE;
            if (imapd_index)
                prot_printf(imapd_out, "* OK [HIGHESTMODSEQ " MODSEQ_FMT "]  \r\n",
                        index_highestmodseq(imapd_index));
        }
    }

    if (fa->fetchitems & (FETCH_ANNOTATION|FETCH_FOLDER)) {
        fa->namespace = &imapd_namespace;
        fa->userid = imapd_userid;
    }
    if (fa->fetchitems & FETCH_ANNOTATION) {
        fa->isadmin = imapd_userisadmin || imapd_userisproxyadmin;
        fa->authstate = imapd_authstate;
    }

    strarray_free(newfields);
    return 0;

freeargs:
    strarray_free(newfields);
    return IMAP_PROTOCOL_BAD_PARAMETERS;
}
