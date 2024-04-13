static int getlistretopts(char *tag, struct listargs *args)
{
    static struct buf buf;
    int c;

    c = getword(imapd_in, &buf);
    if (!*buf.s) {
        prot_printf(imapd_out,
                    "%s BAD Invalid syntax in List command\r\n", tag);
        return EOF;
    }
    lcase(buf.s);
    if (strcasecmp(buf.s, "return")) {
        prot_printf(imapd_out,
                    "%s BAD Unexpected extra argument to List: \"%s\"\r\n",
                    tag, buf.s);
        return EOF;
    }

    if (c != ' ' || (c = prot_getc(imapd_in)) != '(') {
        prot_printf(imapd_out,
                    "%s BAD Missing return argument list\r\n", tag);
        return EOF;
    }

    if ( (c = prot_getc(imapd_in)) == ')')
        return prot_getc(imapd_in);
    else
        prot_ungetc(c, imapd_in);

    for (;;) {
        c = getword(imapd_in, &buf);

        if (!*buf.s) {
            prot_printf(imapd_out,
                        "%s BAD Invalid syntax in List command\r\n", tag);
            return EOF;
        }

        lcase(buf.s);

        if (!strcmp(buf.s, "subscribed"))
            args->ret |= LIST_RET_SUBSCRIBED;
        else if (!strcmp(buf.s, "children"))
            args->ret |= LIST_RET_CHILDREN;
        else if (!strcmp(buf.s, "myrights"))
            args->ret |= LIST_RET_MYRIGHTS;
        else if (!strcmp(buf.s, "special-use"))
            args->ret |= LIST_RET_SPECIALUSE;
        else if (!strcmp(buf.s, "status")) {
            const char *errstr = "Bad status string";
            args->ret |= LIST_RET_STATUS;
            c = parse_statusitems(&args->statusitems, &errstr);
            if (c == EOF) {
                prot_printf(imapd_out, "%s BAD %s", tag, errstr);
                return EOF;
            }
        }
        else if (!strcmp(buf.s, "metadata")) {
            args->ret |= LIST_RET_METADATA;
            /* outputs the error for us */
            c = parse_metadata_string_or_list(tag, &args->metaitems, NULL);
            if (c == EOF) return EOF;
        }
        else {
            prot_printf(imapd_out,
                        "%s BAD Invalid List return option \"%s\"\r\n",
                        tag, buf.s);
            return EOF;
        }

        if (c != ' ') break;
    }

    if (c != ')') {
        prot_printf(imapd_out,
                    "%s BAD Missing close parenthesis for List return options\r\n", tag);
        return EOF;
    }

    return prot_getc(imapd_in);
}
