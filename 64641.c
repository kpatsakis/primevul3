ofputil_pull_ofp14_port_stats(struct ofputil_port_stats *ops,
                              struct ofpbuf *msg)
{
    const struct ofp14_port_stats *ps14 = ofpbuf_try_pull(msg, sizeof *ps14);
    if (!ps14) {
        return OFPERR_OFPBRC_BAD_LEN;
    }

    size_t len = ntohs(ps14->length);
    if (len < sizeof *ps14 || len - sizeof *ps14 > msg->size) {
        return OFPERR_OFPBRC_BAD_LEN;
    }
    len -= sizeof *ps14;

    enum ofperr error = ofputil_port_from_ofp11(ps14->port_no, &ops->port_no);
    if (error) {
        return error;
    }

    ops->duration_sec = ntohl(ps14->duration_sec);
    ops->duration_nsec = ntohl(ps14->duration_nsec);
    ops->stats.rx_packets = ntohll(ps14->rx_packets);
    ops->stats.tx_packets = ntohll(ps14->tx_packets);
    ops->stats.rx_bytes = ntohll(ps14->rx_bytes);
    ops->stats.tx_bytes = ntohll(ps14->tx_bytes);
    ops->stats.rx_dropped = ntohll(ps14->rx_dropped);
    ops->stats.tx_dropped = ntohll(ps14->tx_dropped);
    ops->stats.rx_errors = ntohll(ps14->rx_errors);
    ops->stats.tx_errors = ntohll(ps14->tx_errors);


    struct ofpbuf properties = ofpbuf_const_initializer(ofpbuf_pull(msg, len),
                                                        len);
    while (properties.size > 0) {
        struct ofpbuf payload;
        enum ofperr error;
        uint64_t type = 0;

        error = ofpprop_pull(&properties, &payload, &type);
        if (error) {
            return error;
        }
        switch (type) {
        case OFPPSPT14_ETHERNET:
            error = parse_ofp14_port_stats_ethernet_property(&payload, ops);
            break;
        case OFPPROP_EXP(INTEL_VENDOR_ID, INTEL_PORT_STATS_RFC2819):
            error = parse_intel_port_stats_property(&payload,
                                                    INTEL_PORT_STATS_RFC2819,
                                                    ops);
            break;
        default:
            error = OFPPROP_UNKNOWN(true, "port stats", type);
            break;
        }

        if (error) {
            return error;
        }
    }

    return 0;
}
