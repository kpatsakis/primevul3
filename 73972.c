static void cmd_capability(char *tag)
{
    imapd_check(NULL, 0);

    prot_printf(imapd_out, "* CAPABILITY ");

    capa_response(CAPA_PREAUTH|CAPA_POSTAUTH);

    prot_printf(imapd_out, "\r\n%s OK %s\r\n", tag,
                error_message(IMAP_OK_COMPLETED));
}
