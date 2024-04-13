static int apply_cb(struct findall_data *data, void* rock)
{
    if (!data) return 0;
    struct apply_rock *arock = (struct apply_rock *)rock;
    annotate_state_t *state = arock->state;
    int r;

    /* Suppress any output of a partial match */
    if (!data->mbname)
        return 0;

    strlcpy(arock->lastname, mbname_intname(data->mbname), sizeof(arock->lastname));

    /* NOTE: this is a fricking horrible abuse of layers.  We'll be passing the
     * extname less horribly one day */
    const char *extname = mbname_extname(data->mbname, &imapd_namespace, imapd_userid);
    mbentry_t *backdoor = (mbentry_t *)data->mbentry;
    backdoor->ext_name = xmalloc(strlen(extname)+1);
    strcpy(backdoor->ext_name, extname);

    r = annotate_state_set_mailbox_mbe(state, data->mbentry);
    if (r) return r;

    r = arock->proc(state, arock->data);
    arock->nseen++;

    return r;
}
