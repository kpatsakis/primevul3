static int getresult(struct protstream *p, const char *tag)
{
    char buf[4096];
    char *str = (char *) buf;

    while(1) {
        if (!prot_fgets(str, sizeof(buf), p)) {
            return IMAP_SERVER_UNAVAILABLE;
        }
        if (!strncmp(str, tag, strlen(tag))) {
            str += strlen(tag);
            if(!*str) {
                /* We got a tag, but no response */
                return IMAP_SERVER_UNAVAILABLE;
            }
            str++;
            if (!strncasecmp(str, "OK ", 3)) { return 0; }
            if (!strncasecmp(str, "NO ", 3)) { return IMAP_REMOTE_DENIED; }
            return IMAP_SERVER_UNAVAILABLE; /* huh? */
        }
        /* skip this line, we don't really care */
    }
}
