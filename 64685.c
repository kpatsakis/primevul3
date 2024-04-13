parse_intel_port_stats_rfc2819_property(const struct ofpbuf *payload,
                                        struct ofputil_port_stats *ops)
{
    const struct intel_port_stats_rfc2819 *rfc2819 = payload->data;

    if (payload->size != sizeof *rfc2819) {
        return OFPERR_OFPBPC_BAD_LEN;
    }
    ops->stats.rx_1_to_64_packets = ntohll(rfc2819->rx_1_to_64_packets);
    ops->stats.rx_65_to_127_packets = ntohll(rfc2819->rx_65_to_127_packets);
    ops->stats.rx_128_to_255_packets = ntohll(rfc2819->rx_128_to_255_packets);
    ops->stats.rx_256_to_511_packets = ntohll(rfc2819->rx_256_to_511_packets);
    ops->stats.rx_512_to_1023_packets =
        ntohll(rfc2819->rx_512_to_1023_packets);
    ops->stats.rx_1024_to_1522_packets =
        ntohll(rfc2819->rx_1024_to_1522_packets);
    ops->stats.rx_1523_to_max_packets =
        ntohll(rfc2819->rx_1523_to_max_packets);

    ops->stats.tx_1_to_64_packets = ntohll(rfc2819->tx_1_to_64_packets);
    ops->stats.tx_65_to_127_packets = ntohll(rfc2819->tx_65_to_127_packets);
    ops->stats.tx_128_to_255_packets = ntohll(rfc2819->tx_128_to_255_packets);
    ops->stats.tx_256_to_511_packets = ntohll(rfc2819->tx_256_to_511_packets);
    ops->stats.tx_512_to_1023_packets =
        ntohll(rfc2819->tx_512_to_1023_packets);
    ops->stats.tx_1024_to_1522_packets =
        ntohll(rfc2819->tx_1024_to_1522_packets);
    ops->stats.tx_1523_to_max_packets =
        ntohll(rfc2819->tx_1523_to_max_packets);

    ops->stats.tx_multicast_packets = ntohll(rfc2819->tx_multicast_packets);
    ops->stats.rx_broadcast_packets = ntohll(rfc2819->rx_broadcast_packets);
    ops->stats.tx_broadcast_packets = ntohll(rfc2819->tx_broadcast_packets);
    ops->stats.rx_undersized_errors = ntohll(rfc2819->rx_undersized_errors);

    ops->stats.rx_oversize_errors = ntohll(rfc2819->rx_oversize_errors);
    ops->stats.rx_fragmented_errors = ntohll(rfc2819->rx_fragmented_errors);
    ops->stats.rx_jabber_errors = ntohll(rfc2819->rx_jabber_errors);

    return 0;
}
