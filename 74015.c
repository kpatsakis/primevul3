void cmd_xconvfetch(const char *tag)
{
    int c = ' ';
    struct fetchargs fetchargs;
    int r;
    clock_t start = clock();
    modseq_t ifchangedsince = 0;
    char mytime[100];
    struct dlist *cidlist = NULL;
    struct dlist *item;

    if (backend_current) {
        /* remote mailbox */
        prot_printf(backend_current->out, "%s XCONVFETCH ", tag);
        if (!pipe_command(backend_current, 65536)) {
            pipe_including_tag(backend_current, tag, 0);
        }
        return;
    }

    if (!config_getswitch(IMAPOPT_CONVERSATIONS)) {
        prot_printf(imapd_out, "%s BAD Unrecognized command\r\n", tag);
        eatline(imapd_in, c);
        return;
    }

    /* local mailbox */
    memset(&fetchargs, 0, sizeof(struct fetchargs));

    c = dlist_parse_asatomlist(&cidlist, 0, imapd_in);
    if (c != ' ')
        goto syntax_error;

    /* check CIDs */
    for (item = cidlist->head; item; item = item->next) {
        if (!dlist_ishex64(item)) {
            prot_printf(imapd_out, "%s BAD Invalid CID\r\n", tag);
            eatline(imapd_in, c);
            goto freeargs;
        }
    }

    c = getmodseq(imapd_in, &ifchangedsince);
    if (c != ' ')
        goto syntax_error;

    r = parse_fetch_args(tag, "Xconvfetch", 0, &fetchargs);
    if (r)
        goto freeargs;
    fetchargs.fetchitems |= (FETCH_UIDVALIDITY|FETCH_FOLDER);
    fetchargs.namespace = &imapd_namespace;
    fetchargs.userid = imapd_userid;

    r = do_xconvfetch(cidlist, ifchangedsince, &fetchargs);

    snprintf(mytime, sizeof(mytime), "%2.3f",
             (clock() - start) / (double) CLOCKS_PER_SEC);

    if (r) {
        prot_printf(imapd_out, "%s NO %s (%s sec)\r\n", tag,
                    error_message(r), mytime);
    } else {
        prot_printf(imapd_out, "%s OK Completed (%s sec)\r\n",
                    tag, mytime);
    }

freeargs:
    dlist_free(&cidlist);
    fetchargs_fini(&fetchargs);
    return;

syntax_error:
    prot_printf(imapd_out, "%s BAD Syntax error\r\n", tag);
    eatline(imapd_in, c);
    dlist_free(&cidlist);
    fetchargs_fini(&fetchargs);
}
