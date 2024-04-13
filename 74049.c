static int getsortcriteria(char *tag, struct sortcrit **sortcrit)
{
    int c;
    static struct buf criteria;
    int nsort, n;
    int hasconv = config_getswitch(IMAPOPT_CONVERSATIONS);

    *sortcrit = NULL;

    c = prot_getc(imapd_in);
    if (c != '(') goto missingcrit;

    c = getword(imapd_in, &criteria);
    if (criteria.s[0] == '\0') goto missingcrit;

    nsort = 0;
    n = 0;
    for (;;) {
        if (n >= nsort - 1) {   /* leave room for implicit criterion */
            /* (Re)allocate an array for sort criteria */
            nsort += SORTGROWSIZE;
            *sortcrit =
                (struct sortcrit *) xrealloc(*sortcrit,
                                             nsort * sizeof(struct sortcrit));
            /* Zero out the newly added sortcrit */
            memset((*sortcrit)+n, 0, SORTGROWSIZE * sizeof(struct sortcrit));
        }

        lcase(criteria.s);
        if (!strcmp(criteria.s, "reverse")) {
            (*sortcrit)[n].flags |= SORT_REVERSE;
            goto nextcrit;
        }
        else if (!strcmp(criteria.s, "arrival"))
            (*sortcrit)[n].key = SORT_ARRIVAL;
        else if (!strcmp(criteria.s, "cc"))
            (*sortcrit)[n].key = SORT_CC;
        else if (!strcmp(criteria.s, "date"))
            (*sortcrit)[n].key = SORT_DATE;
        else if (!strcmp(criteria.s, "displayfrom"))
            (*sortcrit)[n].key = SORT_DISPLAYFROM;
        else if (!strcmp(criteria.s, "displayto"))
            (*sortcrit)[n].key = SORT_DISPLAYTO;
        else if (!strcmp(criteria.s, "from"))
            (*sortcrit)[n].key = SORT_FROM;
        else if (!strcmp(criteria.s, "size"))
            (*sortcrit)[n].key = SORT_SIZE;
        else if (!strcmp(criteria.s, "subject"))
            (*sortcrit)[n].key = SORT_SUBJECT;
        else if (!strcmp(criteria.s, "to"))
            (*sortcrit)[n].key = SORT_TO;
        else if (!strcmp(criteria.s, "annotation")) {
            const char *userid = NULL;

            (*sortcrit)[n].key = SORT_ANNOTATION;
            if (c != ' ') goto missingarg;
            c = getastring(imapd_in, imapd_out, &criteria);
            if (c != ' ') goto missingarg;
            (*sortcrit)[n].args.annot.entry = xstrdup(criteria.s);
            c = getastring(imapd_in, imapd_out, &criteria);
            if (c == EOF) goto missingarg;
            if (!strcmp(criteria.s, "value.shared"))
                userid = "";
            else if (!strcmp(criteria.s, "value.priv"))
                userid = imapd_userid;
            else
                goto missingarg;
            (*sortcrit)[n].args.annot.userid = xstrdup(userid);
        }
        else if (!strcmp(criteria.s, "modseq"))
            (*sortcrit)[n].key = SORT_MODSEQ;
        else if (!strcmp(criteria.s, "uid"))
            (*sortcrit)[n].key = SORT_UID;
        else if (!strcmp(criteria.s, "hasflag")) {
            (*sortcrit)[n].key = SORT_HASFLAG;
            if (c != ' ') goto missingarg;
            c = getastring(imapd_in, imapd_out, &criteria);
            if (c == EOF) goto missingarg;
            (*sortcrit)[n].args.flag.name = xstrdup(criteria.s);
        }
        else if (hasconv && !strcmp(criteria.s, "convmodseq"))
            (*sortcrit)[n].key = SORT_CONVMODSEQ;
        else if (hasconv && !strcmp(criteria.s, "convexists"))
            (*sortcrit)[n].key = SORT_CONVEXISTS;
        else if (hasconv && !strcmp(criteria.s, "convsize"))
            (*sortcrit)[n].key = SORT_CONVSIZE;
        else if (hasconv && !strcmp(criteria.s, "hasconvflag")) {
            (*sortcrit)[n].key = SORT_HASCONVFLAG;
            if (c != ' ') goto missingarg;
            c = getastring(imapd_in, imapd_out, &criteria);
            if (c == EOF) goto missingarg;
            (*sortcrit)[n].args.flag.name = xstrdup(criteria.s);
        }
        else if (!strcmp(criteria.s, "folder"))
            (*sortcrit)[n].key = SORT_FOLDER;
        else if (!strcmp(criteria.s, "relevancy"))
            (*sortcrit)[n].key = SORT_RELEVANCY;
        else if (!strcmp(criteria.s, "spamscore"))
            (*sortcrit)[n].key = SORT_SPAMSCORE;
        else {
            prot_printf(imapd_out, "%s BAD Invalid Sort criterion %s\r\n",
                        tag, criteria.s);
            if (c != EOF) prot_ungetc(c, imapd_in);
            return EOF;
        }

        n++;

 nextcrit:
        if (c == ' ') c = getword(imapd_in, &criteria);
        else break;
    }

    if ((*sortcrit)[n].flags & SORT_REVERSE  && !(*sortcrit)[n].key) {
        prot_printf(imapd_out,
                    "%s BAD Missing Sort criterion to reverse\r\n", tag);
        if (c != EOF) prot_ungetc(c, imapd_in);
        return EOF;
    }

    if (c != ')') {
        prot_printf(imapd_out,
                    "%s BAD Missing close parenthesis in Sort\r\n", tag);
        if (c != EOF) prot_ungetc(c, imapd_in);
        return EOF;
    }

    /* Terminate the list with the implicit sort criterion */
    (*sortcrit)[n++].key = SORT_SEQUENCE;

    c = prot_getc(imapd_in);

    return c;

 missingcrit:
    prot_printf(imapd_out, "%s BAD Missing Sort criteria\r\n", tag);
    if (c != EOF) prot_ungetc(c, imapd_in);
    return EOF;
 missingarg:
    prot_printf(imapd_out, "%s BAD Missing argument to Sort criterion %s\r\n",
                tag, criteria.s);
    if (c != EOF) prot_ungetc(c, imapd_in);
    return EOF;
}
