ofproto_port_get_stats(const struct ofport *port, struct netdev_stats *stats)
{
    struct ofproto *ofproto = port->ofproto;
    int error;

    if (ofproto->ofproto_class->port_get_stats) {
        error = ofproto->ofproto_class->port_get_stats(port, stats);
    } else {
        error = EOPNOTSUPP;
    }

    return error;
}
