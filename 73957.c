static int annot_fetch_cb(annotate_state_t *astate, void *rock)
{
    struct annot_fetch_rock *arock = rock;
    return annotate_state_fetch(astate, arock->entries, arock->attribs,
                                arock->callback, arock->cbrock);
}
