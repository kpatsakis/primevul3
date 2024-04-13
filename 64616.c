ofputil_port_stats_from_ofp10(struct ofputil_port_stats *ops,
                              const struct ofp10_port_stats *ps10)
{

    ops->port_no = u16_to_ofp(ntohs(ps10->port_no));
    ops->stats.rx_packets = ntohll(get_32aligned_be64(&ps10->rx_packets));
    ops->stats.tx_packets = ntohll(get_32aligned_be64(&ps10->tx_packets));
    ops->stats.rx_bytes = ntohll(get_32aligned_be64(&ps10->rx_bytes));
    ops->stats.tx_bytes = ntohll(get_32aligned_be64(&ps10->tx_bytes));
    ops->stats.rx_dropped = ntohll(get_32aligned_be64(&ps10->rx_dropped));
    ops->stats.tx_dropped = ntohll(get_32aligned_be64(&ps10->tx_dropped));
    ops->stats.rx_errors = ntohll(get_32aligned_be64(&ps10->rx_errors));
    ops->stats.tx_errors = ntohll(get_32aligned_be64(&ps10->tx_errors));
    ops->stats.rx_frame_errors =
        ntohll(get_32aligned_be64(&ps10->rx_frame_err));
    ops->stats.rx_over_errors = ntohll(get_32aligned_be64(&ps10->rx_over_err));
    ops->stats.rx_crc_errors = ntohll(get_32aligned_be64(&ps10->rx_crc_err));
    ops->stats.collisions = ntohll(get_32aligned_be64(&ps10->collisions));
    ops->duration_sec = ops->duration_nsec = UINT32_MAX;

    return 0;
}
