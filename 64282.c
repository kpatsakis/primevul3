ofport_is_internal_or_patch(const struct ofproto *p, const struct ofport *port)
{
    return !strcmp(netdev_get_type(port->netdev),
                   ofproto_port_open_type(p->type, "internal")) ||
           !strcmp(netdev_get_type(port->netdev),
                   ofproto_port_open_type(p->type, "patch"));
}
