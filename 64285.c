ofport_open(struct ofproto *ofproto,
            struct ofproto_port *ofproto_port,
            struct ofputil_phy_port *pp)
{
    enum netdev_flags flags;
    struct netdev *netdev;
    int error;

    error = netdev_open(ofproto_port->name, ofproto_port->type, &netdev);
    if (error) {
        VLOG_WARN_RL(&rl, "%s: ignoring port %s (%"PRIu32") because netdev %s "
                     "cannot be opened (%s)",
                     ofproto->name,
                     ofproto_port->name, ofproto_port->ofp_port,
                     ofproto_port->name, ovs_strerror(error));
        return NULL;
    }

    if (ofproto_port->ofp_port == OFPP_NONE) {
        if (!strcmp(ofproto->name, ofproto_port->name)) {
            ofproto_port->ofp_port = OFPP_LOCAL;
        } else {
            ofproto_port->ofp_port = alloc_ofp_port(ofproto,
                                                    ofproto_port->name);
        }
    }
    pp->port_no = ofproto_port->ofp_port;
    netdev_get_etheraddr(netdev, &pp->hw_addr);
    ovs_strlcpy(pp->name, ofproto_port->name, sizeof pp->name);
    netdev_get_flags(netdev, &flags);
    pp->config = flags & NETDEV_UP ? 0 : OFPUTIL_PC_PORT_DOWN;
    pp->state = netdev_get_carrier(netdev) ? 0 : OFPUTIL_PS_LINK_DOWN;
    netdev_get_features(netdev, &pp->curr, &pp->advertised,
                        &pp->supported, &pp->peer);
    pp->curr_speed = netdev_features_to_bps(pp->curr, 0) / 1000;
    pp->max_speed = netdev_features_to_bps(pp->supported, 0) / 1000;

    return netdev;
}
