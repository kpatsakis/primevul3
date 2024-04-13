parse_ofp14_port_ethernet_property(const struct ofpbuf *payload,
                                   struct ofputil_phy_port *pp)
{
    struct ofp14_port_desc_prop_ethernet *eth = payload->data;

    if (payload->size != sizeof *eth) {
        return OFPERR_OFPBPC_BAD_LEN;
    }

    pp->curr = netdev_port_features_from_ofp11(eth->curr);
    pp->advertised = netdev_port_features_from_ofp11(eth->advertised);
    pp->supported = netdev_port_features_from_ofp11(eth->supported);
    pp->peer = netdev_port_features_from_ofp11(eth->peer);

    pp->curr_speed = ntohl(eth->curr_speed);
    pp->max_speed = ntohl(eth->max_speed);

    return 0;
}
