static void cmd_genurlauth(char *tag)
{
    struct mboxkey *mboxkey_db;
    int first = 1;
    int c, r;
    static struct buf arg1, arg2;
    struct imapurl url;
    char newkey[MBOX_KEY_LEN];
    char *urlauth = NULL;
    const char *key;
    size_t keylen;
    unsigned char token[EVP_MAX_MD_SIZE+1]; /* +1 for algorithm */
    unsigned int token_len;
    mbentry_t *mbentry = NULL;
    time_t now = time(NULL);

    r = mboxkey_open(imapd_userid, MBOXKEY_CREATE, &mboxkey_db);
    if (r) {
        prot_printf(imapd_out,
                   "%s NO Cannot open mailbox key db for %s: %s\r\n",
                   tag, imapd_userid, error_message(r));
        return;
    }

    do {
        char *intname = NULL;

        c = getastring(imapd_in, imapd_out, &arg1);
        if (c != ' ') {
            prot_printf(imapd_out,
                        "%s BAD Missing required argument to Genurlauth\r\n",
                        tag);
            eatline(imapd_in, c);
            return;
        }
        c = getword(imapd_in, &arg2);
        if (strcasecmp(arg2.s, "INTERNAL")) {
            prot_printf(imapd_out,
                        "%s BAD Unknown auth mechanism to Genurlauth %s\r\n",
                        tag, arg2.s);
            eatline(imapd_in, c);
            return;
        }

        r = imapurl_fromURL(&url, arg1.s);

        /* validate the URL */
        if (r || !url.user || !url.server || !url.mailbox || !url.uid ||
            (url.section && !*url.section) || !url.urlauth.access) {
            r = IMAP_BADURL;
        } else if (strcmp(url.user, imapd_userid)) {
            /* not using currently authorized user's namespace */
            r = IMAP_BADURL;
        } else if (strcmp(url.server, config_servername)) {
            /* wrong server */
            r = IMAP_BADURL;
        } else if (url.urlauth.expire &&
                   url.urlauth.expire < mktime(gmtime(&now))) {
            /* already expired */
            r = IMAP_BADURL;
        }
        if (r) goto err;

        intname = mboxname_from_external(url.mailbox, &imapd_namespace, imapd_userid);
        r = mlookup(NULL, NULL, intname, &mbentry);

        if (r) {
            prot_printf(imapd_out,
                        "%s BAD Poorly specified URL to Genurlauth %s\r\n",
                        tag, arg1.s);
            eatline(imapd_in, c);
            free(url.freeme);
            free(intname);
            return;
        }

        if (mbentry->mbtype & MBTYPE_REMOTE) {
            /* XXX  proxy to backend */
            mboxlist_entry_free(&mbentry);
            free(url.freeme);
            free(intname);
            continue;
        }

        mboxlist_entry_free(&mbentry);

        /* lookup key */
        r = mboxkey_read(mboxkey_db, intname, &key, &keylen);
        if (r) {
            syslog(LOG_ERR, "DBERROR: error fetching mboxkey: %s",
                   cyrusdb_strerror(r));
        }
        else if (!key) {
            /* create a new key */
            RAND_bytes((unsigned char *) newkey, MBOX_KEY_LEN);
            key = newkey;
            keylen = MBOX_KEY_LEN;
            r = mboxkey_write(mboxkey_db, intname, key, keylen);
            if (r) {
                syslog(LOG_ERR, "DBERROR: error writing new mboxkey: %s",
                       cyrusdb_strerror(r));
            }
        }

        if (r) {
        err:
            eatline(imapd_in, c);
            prot_printf(imapd_out,
                        "%s NO Error authorizing %s: %s\r\n",
                        tag, arg1.s, cyrusdb_strerror(r));
            free(url.freeme);
            free(intname);
            return;
        }

        /* first byte is the algorithm used to create token */
        token[0] = URLAUTH_ALG_HMAC_SHA1;
        HMAC(EVP_sha1(), key, keylen, (unsigned char *) arg1.s, strlen(arg1.s),
             token+1, &token_len);
        token_len++;

        urlauth = xrealloc(urlauth, strlen(arg1.s) + 10 +
                           2 * (EVP_MAX_MD_SIZE+1) + 1);
        strcpy(urlauth, arg1.s);
        strcat(urlauth, ":internal:");
        bin_to_hex(token, token_len, urlauth+strlen(urlauth), BH_LOWER);

        if (first) {
            prot_printf(imapd_out, "* GENURLAUTH");
            first = 0;
        }
        (void)prot_putc(' ', imapd_out);
        prot_printstring(imapd_out, urlauth);
        free(intname);
        free(url.freeme);
    } while (c == ' ');

    if (!first) prot_printf(imapd_out, "\r\n");

    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out,
                    "%s BAD Unexpected extra arguments to GENURLAUTH\r\n", tag);
        eatline(imapd_in, c);
    }
    else {
        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
    }

    free(urlauth);

    mboxkey_close(mboxkey_db);
}
