ofputil_decode_aggregate_stats_reply(struct ofputil_aggregate_stats *stats,
                                     const struct ofp_header *reply)
{
    struct ofpbuf msg = ofpbuf_const_initializer(reply, ntohs(reply->length));
    ofpraw_pull_assert(&msg);

    struct ofp_aggregate_stats_reply *asr = msg.msg;
    stats->packet_count = ntohll(get_32aligned_be64(&asr->packet_count));
    stats->byte_count = ntohll(get_32aligned_be64(&asr->byte_count));
    stats->flow_count = ntohl(asr->flow_count);

    return 0;
}
