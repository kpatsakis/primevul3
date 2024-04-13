ofproto_type_get_memory_usage(const char *datapath_type, struct simap *usage)
{
    const struct ofproto_class *class;

    datapath_type = ofproto_normalize_type(datapath_type);
    class = ofproto_class_find__(datapath_type);

    if (class && class->type_get_memory_usage) {
        class->type_get_memory_usage(datapath_type, usage);
    }
}
