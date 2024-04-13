ofputil_queue_stats_to_ofp11(const struct ofputil_queue_stats *oqs,
                             struct ofp11_queue_stats *qs11)
{
    qs11->port_no = ofputil_port_to_ofp11(oqs->port_no);
    qs11->queue_id = htonl(oqs->queue_id);
    qs11->tx_bytes = htonll(oqs->tx_bytes);
    qs11->tx_packets = htonll(oqs->tx_packets);
    qs11->tx_errors = htonll(oqs->tx_errors);
}
