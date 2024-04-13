ofport_is_mtu_overridden(const struct ofproto *p, const struct ofport *port)
{
    return ofport_is_internal_or_patch(p, port)
           && !netdev_mtu_is_user_config(port->netdev);
}
