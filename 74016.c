void cmd_xconvmeta(const char *tag)
{
    int r;
    int c = ' ';
    struct conversations_state *state = NULL;
    struct dlist *cidlist = NULL;
    struct dlist *itemlist = NULL;

    if (backend_current) {
        /* remote mailbox */
        prot_printf(backend_current->out, "%s XCONVMETA ", tag);
        if (!pipe_command(backend_current, 65536)) {
            pipe_including_tag(backend_current, tag, 0);
        }
        return;
    }

    if (!config_getswitch(IMAPOPT_CONVERSATIONS)) {
        prot_printf(imapd_out, "%s BAD Unrecognized command\r\n", tag);
        eatline(imapd_in, c);
        goto done;
    }

    c = dlist_parse_asatomlist(&cidlist, 0, imapd_in);
    if (c != ' ') {
        prot_printf(imapd_out, "%s BAD Failed to parse CID list\r\n", tag);
        eatline(imapd_in, c);
        goto done;
    }

    c = dlist_parse_asatomlist(&itemlist, 0, imapd_in);
    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out, "%s BAD Failed to parse item list\r\n", tag);
        eatline(imapd_in, c);
        goto done;
    }

    r = conversations_open_user(imapd_userid, &state);
    if (r) {
        prot_printf(imapd_out, "%s BAD failed to open db: %s\r\n",
                    tag, error_message(r));
        goto done;
    }

    do_xconvmeta(tag, state, cidlist, itemlist);

 done:

    dlist_free(&itemlist);
    dlist_free(&cidlist);
    conversations_commit(&state);
}
