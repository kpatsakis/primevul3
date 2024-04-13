ofputil_queue_stats_from_ofp11(struct ofputil_queue_stats *oqs,
                               const struct ofp11_queue_stats *qs11)
{
    enum ofperr error;

    error = ofputil_port_from_ofp11(qs11->port_no, &oqs->port_no);
    if (error) {
        return error;
    }

    oqs->queue_id = ntohl(qs11->queue_id);
    oqs->tx_bytes = ntohll(qs11->tx_bytes);
    oqs->tx_packets = ntohll(qs11->tx_packets);
    oqs->tx_errors = ntohll(qs11->tx_errors);
    oqs->duration_sec = oqs->duration_nsec = UINT32_MAX;

    return 0;
}
