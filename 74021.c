static void cmd_xkillmy(const char *tag, const char *cmdname)
{
    char *cmd = xstrdup(cmdname);
    char *p;

    /* normalise to imapd conventions */
    if (Uislower(cmd[0]))
        cmd[0] = toupper((unsigned char) cmd[0]);
    for (p = cmd+1; *p; p++) {
        if (Uisupper(*p)) *p = tolower((unsigned char) *p);
    }

    proc_killusercmd(imapd_userid, cmd, SIGUSR2);

    free(cmd);

    prot_printf(imapd_out, "%s OK %s\r\n", tag,
                error_message(IMAP_OK_COMPLETED));
}
