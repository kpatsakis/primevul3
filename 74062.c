static void motd_file(void)
{
    char *filename = NULL;
    int fd = -1;
    struct protstream *motd_in = NULL;
    char buf[MAX_MAILBOX_PATH+1];
    char *p;

    filename = strconcat(config_dir, "/msg/motd", (char *)NULL);
    fd = open(filename, O_RDONLY, 0);
    if (fd < 0)
        goto out;

    motd_in = prot_new(fd, 0);

    prot_fgets(buf, sizeof(buf), motd_in);
    if ((p = strchr(buf, '\r'))!=NULL) *p = 0;
    if ((p = strchr(buf, '\n'))!=NULL) *p = 0;

    for (p = buf; *p == '['; p++); /* can't have [ be first char, sigh */
    prot_printf(imapd_out, "* OK [ALERT] %s\r\n", p);

out:
    if (motd_in)
        prot_free(motd_in);
    if (fd >= 0)
        close(fd);
    free(filename);
}
