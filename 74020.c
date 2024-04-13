static void cmd_xforever(const char *tag)
{
    unsigned n = 1;
    int r = 0;

    while (!r) {
        sleep(1);
        prot_printf(imapd_out, "* FOREVER %u\r\n", n++);
        prot_flush(imapd_out);
        r = cmd_cancelled();
    }

    prot_printf(imapd_out, "%s OK %s\r\n", tag, error_message(r));
}
