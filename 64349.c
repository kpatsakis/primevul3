ofproto_port_open_type(const char *datapath_type, const char *port_type)
{
    const struct ofproto_class *class;

    datapath_type = ofproto_normalize_type(datapath_type);
    class = ofproto_class_find__(datapath_type);
    if (!class) {
        return port_type;
    }

    return (class->port_open_type
            ? class->port_open_type(datapath_type, port_type)
            : port_type);
}
