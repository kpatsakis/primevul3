ofpacts_copy_all(struct ofpbuf *out, const struct ofpbuf *in,
                 bool (*filter)(const struct ofpact *))
{
    const struct ofpact *a;

    OFPACT_FOR_EACH (a, in->data, in->size) {
        if (filter(a)) {
            ofpact_copy(out, a);
        }
    }
}
