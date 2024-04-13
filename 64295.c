ofproto_class_find__(const char *type)
{
    size_t i;

    for (i = 0; i < n_ofproto_classes; i++) {
        const struct ofproto_class *class = ofproto_classes[i];
        struct sset types;
        bool found;

        sset_init(&types);
        class->enumerate_types(&types);
        found = sset_contains(&types, type);
        sset_destroy(&types);

        if (found) {
            return class;
        }
    }
    VLOG_WARN("unknown datapath type %s", type);
    return NULL;
}
