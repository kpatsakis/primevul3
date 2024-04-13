static void cmd_sort(char *tag, int usinguid)
{
    int c;
    struct sortcrit *sortcrit = NULL;
    struct searchargs *searchargs = NULL;
    clock_t start = clock();
    char mytime[100];
    int n;

    if (backend_current) {
        /* remote mailbox */
        const char *cmd = usinguid ? "UID Sort" : "Sort";

        prot_printf(backend_current->out, "%s %s ", tag, cmd);
        if (!pipe_command(backend_current, 65536)) {
            pipe_including_tag(backend_current, tag, 0);
        }
        return;
    }

    /* local mailbox */
    c = getsortcriteria(tag, &sortcrit);
    if (c == EOF) goto error;

    searchargs = new_searchargs(tag, GETSEARCH_CHARSET_FIRST,
                                &imapd_namespace, imapd_userid, imapd_authstate,
                                imapd_userisadmin || imapd_userisproxyadmin);
    if (imapd_id.quirks & QUIRK_SEARCHFUZZY)
        searchargs->fuzzy_depth++;
    c = get_search_program(imapd_in, imapd_out, searchargs);
    if (c == EOF) goto error;

    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out,
                    "%s BAD Unexpected extra arguments to Sort\r\n", tag);
        goto error;
    }

    n = index_sort(imapd_index, sortcrit, searchargs, usinguid);
    snprintf(mytime, sizeof(mytime), "%2.3f",
             (clock() - start) / (double) CLOCKS_PER_SEC);
    if (CONFIG_TIMING_VERBOSE) {
        char *s = sortcrit_as_string(sortcrit);
        syslog(LOG_DEBUG, "SORT (%s) processing time: %d msg in %s sec",
               s, n, mytime);
        free(s);
    }
    prot_printf(imapd_out, "%s OK %s (%d msgs in %s secs)\r\n", tag,
                error_message(IMAP_OK_COMPLETED), n, mytime);

    freesortcrit(sortcrit);
    freesearchargs(searchargs);
    return;

error:
    eatline(imapd_in, (c == EOF ? ' ' : c));
    freesortcrit(sortcrit);
    freesearchargs(searchargs);
}
