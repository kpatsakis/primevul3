parse_ofp14_port_stats_ethernet_property(const struct ofpbuf *payload,
                                         struct ofputil_port_stats *ops)
{
    const struct ofp14_port_stats_prop_ethernet *eth = payload->data;

    if (payload->size != sizeof *eth) {
        return OFPERR_OFPBPC_BAD_LEN;
    }

    ops->stats.rx_frame_errors = ntohll(eth->rx_frame_err);
    ops->stats.rx_over_errors = ntohll(eth->rx_over_err);
    ops->stats.rx_crc_errors = ntohll(eth->rx_crc_err);
    ops->stats.collisions = ntohll(eth->collisions);

    return 0;
}
