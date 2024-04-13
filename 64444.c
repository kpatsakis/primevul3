ofputil_append_ofp14_port_stats(const struct ofputil_port_stats *ops,
                                struct ovs_list *replies)
{
    struct ofp14_port_stats_prop_ethernet *eth;
    struct intel_port_stats_rfc2819 *stats_rfc2819;
    struct ofp14_port_stats *ps14;
    struct ofpbuf *reply;

    reply = ofpmp_reserve(replies, sizeof *ps14 + sizeof *eth +
                          sizeof *stats_rfc2819);

    ps14 = ofpbuf_put_uninit(reply, sizeof *ps14);
    ps14->length = htons(sizeof *ps14 + sizeof *eth +
                         sizeof *stats_rfc2819);
    memset(ps14->pad, 0, sizeof ps14->pad);
    ps14->port_no = ofputil_port_to_ofp11(ops->port_no);
    ps14->duration_sec = htonl(ops->duration_sec);
    ps14->duration_nsec = htonl(ops->duration_nsec);
    ps14->rx_packets = htonll(ops->stats.rx_packets);
    ps14->tx_packets = htonll(ops->stats.tx_packets);
    ps14->rx_bytes = htonll(ops->stats.rx_bytes);
    ps14->tx_bytes = htonll(ops->stats.tx_bytes);
    ps14->rx_dropped = htonll(ops->stats.rx_dropped);
    ps14->tx_dropped = htonll(ops->stats.tx_dropped);
    ps14->rx_errors = htonll(ops->stats.rx_errors);
    ps14->tx_errors = htonll(ops->stats.tx_errors);

    eth = ofpprop_put_zeros(reply, OFPPSPT14_ETHERNET, sizeof *eth);
    eth->rx_frame_err = htonll(ops->stats.rx_frame_errors);
    eth->rx_over_err = htonll(ops->stats.rx_over_errors);
    eth->rx_crc_err = htonll(ops->stats.rx_crc_errors);
    eth->collisions = htonll(ops->stats.collisions);

    uint64_t prop_type = OFPPROP_EXP(INTEL_VENDOR_ID,
                                     INTEL_PORT_STATS_RFC2819);

    stats_rfc2819 = ofpprop_put_zeros(reply, prop_type,
                                      sizeof *stats_rfc2819);

    memset(stats_rfc2819->pad, 0, sizeof stats_rfc2819->pad);
    stats_rfc2819->rx_1_to_64_packets = htonll(ops->stats.rx_1_to_64_packets);
    stats_rfc2819->rx_65_to_127_packets =
        htonll(ops->stats.rx_65_to_127_packets);
    stats_rfc2819->rx_128_to_255_packets =
        htonll(ops->stats.rx_128_to_255_packets);
    stats_rfc2819->rx_256_to_511_packets =
        htonll(ops->stats.rx_256_to_511_packets);
    stats_rfc2819->rx_512_to_1023_packets =
        htonll(ops->stats.rx_512_to_1023_packets);
    stats_rfc2819->rx_1024_to_1522_packets =
        htonll(ops->stats.rx_1024_to_1522_packets);
    stats_rfc2819->rx_1523_to_max_packets =
        htonll(ops->stats.rx_1523_to_max_packets);

    stats_rfc2819->tx_1_to_64_packets = htonll(ops->stats.tx_1_to_64_packets);
    stats_rfc2819->tx_65_to_127_packets =
        htonll(ops->stats.tx_65_to_127_packets);
    stats_rfc2819->tx_128_to_255_packets =
        htonll(ops->stats.tx_128_to_255_packets);
    stats_rfc2819->tx_256_to_511_packets =
        htonll(ops->stats.tx_256_to_511_packets);
    stats_rfc2819->tx_512_to_1023_packets =
        htonll(ops->stats.tx_512_to_1023_packets);
    stats_rfc2819->tx_1024_to_1522_packets =
        htonll(ops->stats.tx_1024_to_1522_packets);
    stats_rfc2819->tx_1523_to_max_packets =
        htonll(ops->stats.tx_1523_to_max_packets);

    stats_rfc2819->tx_multicast_packets =
        htonll(ops->stats.tx_multicast_packets);
    stats_rfc2819->rx_broadcast_packets =
        htonll(ops->stats.rx_broadcast_packets);
    stats_rfc2819->tx_broadcast_packets =
        htonll(ops->stats.tx_broadcast_packets);
    stats_rfc2819->rx_undersized_errors =
        htonll(ops->stats.rx_undersized_errors);
    stats_rfc2819->rx_oversize_errors =
        htonll(ops->stats.rx_oversize_errors);
    stats_rfc2819->rx_fragmented_errors =
        htonll(ops->stats.rx_fragmented_errors);
    stats_rfc2819->rx_jabber_errors =
        htonll(ops->stats.rx_jabber_errors);
}
