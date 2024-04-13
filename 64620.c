ofputil_port_stats_to_ofp11(const struct ofputil_port_stats *ops,
                            struct ofp11_port_stats *ps11)
{
    ps11->port_no = ofputil_port_to_ofp11(ops->port_no);
    memset(ps11->pad, 0, sizeof ps11->pad);
    ps11->rx_packets = htonll(ops->stats.rx_packets);
    ps11->tx_packets = htonll(ops->stats.tx_packets);
    ps11->rx_bytes = htonll(ops->stats.rx_bytes);
    ps11->tx_bytes = htonll(ops->stats.tx_bytes);
    ps11->rx_dropped = htonll(ops->stats.rx_dropped);
    ps11->tx_dropped = htonll(ops->stats.tx_dropped);
    ps11->rx_errors = htonll(ops->stats.rx_errors);
    ps11->tx_errors = htonll(ops->stats.tx_errors);
    ps11->rx_frame_err = htonll(ops->stats.rx_frame_errors);
    ps11->rx_over_err = htonll(ops->stats.rx_over_errors);
    ps11->rx_crc_err = htonll(ops->stats.rx_crc_errors);
    ps11->collisions = htonll(ops->stats.collisions);
}
