ofputil_put_ofp14_port(const struct ofputil_phy_port *pp,
                       struct ofpbuf *b)
{
    struct ofp14_port *op;
    struct ofp14_port_desc_prop_ethernet *eth;

    ofpbuf_prealloc_tailroom(b, sizeof *op + sizeof *eth);

    op = ofpbuf_put_zeros(b, sizeof *op);
    op->port_no = ofputil_port_to_ofp11(pp->port_no);
    op->length = htons(sizeof *op + sizeof *eth);
    op->hw_addr = pp->hw_addr;
    ovs_strlcpy(op->name, pp->name, sizeof op->name);
    op->config = htonl(pp->config & OFPPC11_ALL);
    op->state = htonl(pp->state & OFPPS11_ALL);

    eth = ofpprop_put_zeros(b, OFPPDPT14_ETHERNET, sizeof *eth);
    eth->curr = netdev_port_features_to_ofp11(pp->curr);
    eth->advertised = netdev_port_features_to_ofp11(pp->advertised);
    eth->supported = netdev_port_features_to_ofp11(pp->supported);
    eth->peer = netdev_port_features_to_ofp11(pp->peer);
    eth->curr_speed = htonl(pp->curr_speed);
    eth->max_speed = htonl(pp->max_speed);
}
