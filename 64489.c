ofputil_decode_ofp10_phy_port(struct ofputil_phy_port *pp,
                              const struct ofp10_phy_port *opp)
{
    pp->port_no = u16_to_ofp(ntohs(opp->port_no));
    pp->hw_addr = opp->hw_addr;
    ovs_strlcpy(pp->name, opp->name, OFP_MAX_PORT_NAME_LEN);

    pp->config = ntohl(opp->config) & OFPPC10_ALL;
    pp->state = ntohl(opp->state) & OFPPS10_ALL;

    pp->curr = netdev_port_features_from_ofp10(opp->curr);
    pp->advertised = netdev_port_features_from_ofp10(opp->advertised);
    pp->supported = netdev_port_features_from_ofp10(opp->supported);
    pp->peer = netdev_port_features_from_ofp10(opp->peer);

    pp->curr_speed = netdev_features_to_bps(pp->curr, 0) / 1000;
    pp->max_speed = netdev_features_to_bps(pp->supported, 0) / 1000;

    return 0;
}
