static void cmd_xmeid(const char *tag, const char *id)
{
    mboxevent_set_client_id(id);

    prot_printf(imapd_out, "%s OK %s\r\n", tag,
                error_message(IMAP_OK_COMPLETED));
}
