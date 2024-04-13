ofputil_port_stats_from_ofp11(struct ofputil_port_stats *ops,
                              const struct ofp11_port_stats *ps11)
{
    enum ofperr error;

    error = ofputil_port_from_ofp11(ps11->port_no, &ops->port_no);
    if (error) {
        return error;
    }

    ops->stats.rx_packets = ntohll(ps11->rx_packets);
    ops->stats.tx_packets = ntohll(ps11->tx_packets);
    ops->stats.rx_bytes = ntohll(ps11->rx_bytes);
    ops->stats.tx_bytes = ntohll(ps11->tx_bytes);
    ops->stats.rx_dropped = ntohll(ps11->rx_dropped);
    ops->stats.tx_dropped = ntohll(ps11->tx_dropped);
    ops->stats.rx_errors = ntohll(ps11->rx_errors);
    ops->stats.tx_errors = ntohll(ps11->tx_errors);
    ops->stats.rx_frame_errors = ntohll(ps11->rx_frame_err);
    ops->stats.rx_over_errors = ntohll(ps11->rx_over_err);
    ops->stats.rx_crc_errors = ntohll(ps11->rx_crc_err);
    ops->stats.collisions = ntohll(ps11->collisions);
    ops->duration_sec = ops->duration_nsec = UINT32_MAX;

    return 0;
}
