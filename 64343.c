ofproto_port_get_lacp_stats(const struct ofport *port, struct lacp_slave_stats *stats)
{
    struct ofproto *ofproto = port->ofproto;
    int error;

    if (ofproto->ofproto_class->port_get_lacp_stats) {
        error = ofproto->ofproto_class->port_get_lacp_stats(port, stats);
    } else {
        error = EOPNOTSUPP;
    }

    return error;
}
