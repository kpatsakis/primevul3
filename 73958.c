static int annot_store_cb(annotate_state_t *astate, void *rock)
{
    struct annot_store_rock *arock = rock;
    return annotate_state_store(astate, arock->entryatts);
}
