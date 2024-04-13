ofproto_enumerate_names(const char *type, struct sset *names)
{
    const struct ofproto_class *class = ofproto_class_find__(type);
    return class ? class->enumerate_names(type, names) : EAFNOSUPPORT;
}
