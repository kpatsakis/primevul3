static int getlistselopts(char *tag, struct listargs *args)
{
    int c;
    static struct buf buf;

    if ( (c = prot_getc(imapd_in)) == ')')
        return prot_getc(imapd_in);
    else
        prot_ungetc(c, imapd_in);

    for (;;) {
        c = getword(imapd_in, &buf);

        if (!*buf.s) {
            prot_printf(imapd_out,
                        "%s BAD Invalid syntax in List command\r\n",
                        tag);
            return EOF;
        }

        lcase(buf.s);

        if (!strcmp(buf.s, "subscribed")) {
            args->sel |= LIST_SEL_SUBSCRIBED;
            args->ret |= LIST_RET_SUBSCRIBED;
        } else if (!strcmp(buf.s, "vendor.cmu-dav")) {
            args->sel |= LIST_SEL_DAV;
        } else if (!strcmp(buf.s, "remote")) {
            args->sel |= LIST_SEL_REMOTE;
        } else if (!strcmp(buf.s, "recursivematch")) {
            args->sel |= LIST_SEL_RECURSIVEMATCH;
        } else if (!strcmp(buf.s, "special-use")) {
            args->sel |= LIST_SEL_SPECIALUSE;
            args->ret |= LIST_RET_SPECIALUSE;
        } else if (!strcmp(buf.s, "metadata")) {
            struct getmetadata_options opts = OPTS_INITIALIZER;
            args->sel |= LIST_SEL_METADATA;
            args->ret |= LIST_RET_METADATA;

            strarray_t options = STRARRAY_INITIALIZER;
            c = parse_metadata_string_or_list(tag, &options, NULL);
            parse_getmetadata_options(&options, &opts);
            args->metaopts = opts;
            strarray_fini(&options);
            if (c == EOF) return EOF;
        } else {
            prot_printf(imapd_out,
                        "%s BAD Invalid List selection option \"%s\"\r\n",
                        tag, buf.s);
            return EOF;
        }

        if (c != ' ') break;
    }

    if (c != ')') {
        prot_printf(imapd_out,
                    "%s BAD Missing close parenthesis for List selection options\r\n", tag);
        return EOF;
    }

    if (args->sel & list_select_mod_opts
            && ! (args->sel & list_select_base_opts)) {
        prot_printf(imapd_out,
                    "%s BAD Invalid combination of selection options\r\n",
                    tag);
        return EOF;
    }

    return prot_getc(imapd_in);
}
