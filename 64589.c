ofputil_group_stats_to_ofp11(const struct ofputil_group_stats *gs,
                             struct ofp11_group_stats *gs11, size_t length,
                             struct ofp11_bucket_counter bucket_cnts[])
{
    memset(gs11, 0, sizeof *gs11);
    gs11->length = htons(length);
    gs11->group_id = htonl(gs->group_id);
    gs11->ref_count = htonl(gs->ref_count);
    gs11->packet_count = htonll(gs->packet_count);
    gs11->byte_count = htonll(gs->byte_count);
    ofputil_group_bucket_counters_to_ofp11(gs, bucket_cnts);
}
