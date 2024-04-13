ofputil_queue_stats_to_ofp10(const struct ofputil_queue_stats *oqs,
                             struct ofp10_queue_stats *qs10)
{
    qs10->port_no = htons(ofp_to_u16(oqs->port_no));
    memset(qs10->pad, 0, sizeof qs10->pad);
    qs10->queue_id = htonl(oqs->queue_id);
    put_32aligned_be64(&qs10->tx_bytes, htonll(oqs->tx_bytes));
    put_32aligned_be64(&qs10->tx_packets, htonll(oqs->tx_packets));
    put_32aligned_be64(&qs10->tx_errors, htonll(oqs->tx_errors));
}
