ofputil_ipfix_stats_to_reply(const struct ofputil_ipfix_stats *ois,
                            struct nx_ipfix_stats_reply *reply)
{
    reply->collector_set_id = htonl(ois->collector_set_id);
    reply->total_flows = htonll(ois->total_flows);
    reply->current_flows = htonll(ois->current_flows);
    reply->pkts = htonll(ois->pkts);
    reply->ipv4_pkts = htonll(ois->ipv4_pkts);
    reply->ipv6_pkts = htonll(ois->ipv6_pkts);
    reply->error_pkts = htonll(ois->error_pkts);
    reply->ipv4_error_pkts = htonll(ois->ipv4_error_pkts);
    reply->ipv6_error_pkts = htonll(ois->ipv6_error_pkts);
    reply->tx_pkts = htonll(ois->tx_pkts);
    reply->tx_errors = htonll(ois->tx_errors);
    memset(reply->pad, 0, sizeof reply->pad);
}
