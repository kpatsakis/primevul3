ofputil_decode_ofp11_port(struct ofputil_phy_port *pp,
                          const struct ofp11_port *op)
{
    enum ofperr error;

    error = ofputil_port_from_ofp11(op->port_no, &pp->port_no);
    if (error) {
        return error;
    }
    pp->hw_addr = op->hw_addr;
    ovs_strlcpy(pp->name, op->name, OFP_MAX_PORT_NAME_LEN);

    pp->config = ntohl(op->config) & OFPPC11_ALL;
    pp->state = ntohl(op->state) & OFPPS11_ALL;

    pp->curr = netdev_port_features_from_ofp11(op->curr);
    pp->advertised = netdev_port_features_from_ofp11(op->advertised);
    pp->supported = netdev_port_features_from_ofp11(op->supported);
    pp->peer = netdev_port_features_from_ofp11(op->peer);

    pp->curr_speed = ntohl(op->curr_speed);
    pp->max_speed = ntohl(op->max_speed);

    return 0;
}
