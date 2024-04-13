ofputil_ipfix_stats_from_nx(struct ofputil_ipfix_stats *is,
                            const struct nx_ipfix_stats_reply *reply)
{
    is->collector_set_id = ntohl(reply->collector_set_id);
    is->total_flows = ntohll(reply->total_flows);
    is->current_flows = ntohll(reply->current_flows);
    is->pkts = ntohll(reply->pkts);
    is->ipv4_pkts = ntohll(reply->ipv4_pkts);
    is->ipv6_pkts = ntohll(reply->ipv6_pkts);
    is->error_pkts = ntohll(reply->error_pkts);
    is->ipv4_error_pkts = ntohll(reply->ipv4_error_pkts);
    is->ipv6_error_pkts = ntohll(reply->ipv6_error_pkts);
    is->tx_pkts = ntohll(reply->tx_pkts);
    is->tx_errors = ntohll(reply->tx_errors);

    return 0;
}
