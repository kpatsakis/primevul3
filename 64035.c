ofpacts_copy_last(struct ofpbuf *out, const struct ofpbuf *in,
                  enum ofpact_type filter)
{
    const struct ofpact *target;
    const struct ofpact *a;

    target = NULL;
    OFPACT_FOR_EACH (a, in->data, in->size) {
        if (a->type == filter) {
            target = a;
        }
    }
    if (target) {
        ofpact_copy(out, target);
    }
    return target != NULL;
}
