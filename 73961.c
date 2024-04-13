static int apply_mailbox_array(annotate_state_t *state,
                               const strarray_t *mboxes,
                               int (*proc)(annotate_state_t *, void *),
                               void *rock)
{
    int i;
    mbentry_t *mbentry = NULL;
    char *intname = NULL;
    int r = 0;

    for (i = 0 ; i < mboxes->count ; i++) {
        intname = mboxname_from_external(strarray_nth(mboxes, i), &imapd_namespace, imapd_userid);

        r = mboxlist_lookup(intname, &mbentry, NULL);
        if (r)
            break;

        r = annotate_state_set_mailbox_mbe(state, mbentry);
        if (r)
            break;

        r = proc(state, rock);
        if (r)
            break;

        mboxlist_entry_free(&mbentry);
        free(intname);
        intname = NULL;
    }

    mboxlist_entry_free(&mbentry);
    free(intname);

    return r;
}
