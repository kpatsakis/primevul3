static int parse_statusitems(unsigned *statusitemsp, const char **errstr)
{
    static struct buf arg;
    unsigned statusitems = 0;
    int c;
    int hasconv = config_getswitch(IMAPOPT_CONVERSATIONS);

    c = prot_getc(imapd_in);
    if (c != '(') return EOF;

    c = getword(imapd_in, &arg);
    if (arg.s[0] == '\0') return EOF;
    for (;;) {
        lcase(arg.s);
        if (!strcmp(arg.s, "messages")) {
            statusitems |= STATUS_MESSAGES;
        }
        else if (!strcmp(arg.s, "recent")) {
            statusitems |= STATUS_RECENT;
        }
        else if (!strcmp(arg.s, "uidnext")) {
            statusitems |= STATUS_UIDNEXT;
        }
        else if (!strcmp(arg.s, "uidvalidity")) {
            statusitems |= STATUS_UIDVALIDITY;
        }
        else if (!strcmp(arg.s, "unseen")) {
            statusitems |= STATUS_UNSEEN;
        }
        else if (!strcmp(arg.s, "highestmodseq")) {
            statusitems |= STATUS_HIGHESTMODSEQ;
        }
        else if (hasconv && !strcmp(arg.s, "xconvexists")) {
            statusitems |= STATUS_XCONVEXISTS;
        }
        else if (hasconv && !strcmp(arg.s, "xconvunseen")) {
            statusitems |= STATUS_XCONVUNSEEN;
        }
        else if (hasconv && !strcmp(arg.s, "xconvmodseq")) {
            statusitems |= STATUS_XCONVMODSEQ;
        }
        else {
            static char buf[200];
            snprintf(buf, 200, "Invalid Status attributes %s", arg.s);
            *errstr = buf;
            return EOF;
        }

        if (c == ' ') c = getword(imapd_in, &arg);
        else break;
    }

    if (c != ')') {
        *errstr = "Missing close parenthesis in Status";
        return EOF;
    }
    c = prot_getc(imapd_in);

    /* success */
    *statusitemsp = statusitems;
    return c;
}
