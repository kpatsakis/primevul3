static int _metadata_to_annotate(const strarray_t *entries,
                                 strarray_t *newa, strarray_t *newe,
                                 const char *tag, int depth)
{
    int i;
    int have_shared = 0;
    int have_private = 0;

    /* we need to rewrite the entries and attribs to match the way that
     * the old annotation system works. */
    for (i = 0 ; i < entries->count ; i++) {
        char *ent = entries->data[i];
        char entry[MAX_MAILBOX_NAME+1];

        lcase(ent);
        /* there's no way to perfect this - unfortunately - the old style
         * syntax doesn't support everything.  XXX - will be nice to get
         * rid of this... */
        if (!strncmp(ent, "/private", 8) &&
            (ent[8] == '\0' || ent[8] == '/')) {
            xstrncpy(entry, ent + 8, MAX_MAILBOX_NAME);
            have_private = 1;
        }
        else if (!strncmp(ent, "/shared", 7) &&
                 (ent[7] == '\0' || ent[7] == '/')) {
            xstrncpy(entry, ent + 7, MAX_MAILBOX_NAME);
            have_shared = 1;
        }
        else {
            if (tag)
                prot_printf(imapd_out,
                            "%s BAD entry must begin with /shared or /private\r\n",
                            tag);
            return IMAP_NO_NOSUCHMSG;
        }
        strarray_append(newe, entry);
        if (depth == 1) {
            strncat(entry, "/%", MAX_MAILBOX_NAME);
            strarray_append(newe, entry);
        }
        else if (depth == -1) {
            strncat(entry, "/*", MAX_MAILBOX_NAME);
            strarray_append(newe, entry);
        }
    }

    if (have_private) strarray_append(newa, "value.priv");
    if (have_shared) strarray_append(newa, "value.shared");

    return 0;
}
