static void imapd_check(struct backend *be, int usinguid)
{
    if (backend_current && backend_current != be) {
        /* remote mailbox */
        char mytag[128];

        proxy_gentag(mytag, sizeof(mytag));

        prot_printf(backend_current->out, "%s Noop\r\n", mytag);
        pipe_until_tag(backend_current, mytag, 0);
    }
    else {
        /* local mailbox */
        index_check(imapd_index, usinguid, 0);
    }
}
