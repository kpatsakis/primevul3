ofputil_pull_ofp14_port(struct ofputil_phy_port *pp, struct ofpbuf *msg)
{
    struct ofp14_port *op = ofpbuf_try_pull(msg, sizeof *op);
    if (!op) {
        return OFPERR_OFPBRC_BAD_LEN;
    }

    size_t len = ntohs(op->length);
    if (len < sizeof *op || len - sizeof *op > msg->size) {
        return OFPERR_OFPBRC_BAD_LEN;
    }
    len -= sizeof *op;

    enum ofperr error = ofputil_port_from_ofp11(op->port_no, &pp->port_no);
    if (error) {
        return error;
    }
    pp->hw_addr = op->hw_addr;
    ovs_strlcpy(pp->name, op->name, OFP_MAX_PORT_NAME_LEN);

    pp->config = ntohl(op->config) & OFPPC11_ALL;
    pp->state = ntohl(op->state) & OFPPS11_ALL;

    struct ofpbuf properties = ofpbuf_const_initializer(ofpbuf_pull(msg, len),
                                                        len);
    while (properties.size > 0) {
        struct ofpbuf payload;
        enum ofperr error;
        uint64_t type;

        error = ofpprop_pull(&properties, &payload, &type);
        if (error) {
            return error;
        }

        switch (type) {
        case OFPPDPT14_ETHERNET:
            error = parse_ofp14_port_ethernet_property(&payload, pp);
            break;

        default:
            error = OFPPROP_UNKNOWN(true, "port", type);
            break;
        }

        if (error) {
            return error;
        }
    }

    return 0;
}
