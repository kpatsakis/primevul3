ofputil_queue_stats_from_ofp10(struct ofputil_queue_stats *oqs,
                               const struct ofp10_queue_stats *qs10)
{
    oqs->port_no = u16_to_ofp(ntohs(qs10->port_no));
    oqs->queue_id = ntohl(qs10->queue_id);
    oqs->tx_bytes = ntohll(get_32aligned_be64(&qs10->tx_bytes));
    oqs->tx_packets = ntohll(get_32aligned_be64(&qs10->tx_packets));
    oqs->tx_errors = ntohll(get_32aligned_be64(&qs10->tx_errors));
    oqs->duration_sec = oqs->duration_nsec = UINT32_MAX;

    return 0;
}
