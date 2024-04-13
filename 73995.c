static void cmd_resetkey(char *tag, char *name,
                  char *mechanism __attribute__((unused)))
/* XXX we don't support any external mechanisms, so we ignore it */
{
    int r;

    if (name) {
        /* delete key for specified mailbox */
        struct mboxkey *mboxkey_db;
        mbentry_t *mbentry = NULL;

        char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);
        r = mlookup(NULL, NULL, intname, &mbentry);
        if (r) {
            prot_printf(imapd_out, "%s NO Error removing key: %s\r\n",
                        tag, error_message(r));
            free(intname);
            return;
        }

        if (mbentry->mbtype & MBTYPE_REMOTE) {
            /* XXX  proxy to backend */
            mboxlist_entry_free(&mbentry);
            free(intname);
            return;
        }

        mboxlist_entry_free(&mbentry);

        r = mboxkey_open(imapd_userid, MBOXKEY_CREATE, &mboxkey_db);
        if (!r) {
            r = mboxkey_write(mboxkey_db, intname, NULL, 0);
            mboxkey_close(mboxkey_db);
        }

        if (r) {
            prot_printf(imapd_out, "%s NO Error removing key: %s\r\n",
                        tag, cyrusdb_strerror(r));
        } else {
            prot_printf(imapd_out,
                        "%s OK [URLMECH INTERNAL] key removed\r\n", tag);
        }
        free(intname);
    }
    else {
        /* delete ALL keys */
        /* XXX  what do we do about multiple backends? */
        r = mboxkey_delete_user(imapd_userid);
        if (r) {
            prot_printf(imapd_out, "%s NO Error removing keys: %s\r\n",
                        tag, cyrusdb_strerror(r));
        } else {
            prot_printf(imapd_out, "%s OK All keys removed\r\n", tag);
        }
    }
}
