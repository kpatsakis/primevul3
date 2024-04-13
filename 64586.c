ofputil_group_bucket_counters_to_ofp11(const struct ofputil_group_stats *gs,
                                    struct ofp11_bucket_counter bucket_cnts[])
{
    int i;

    for (i = 0; i < gs->n_buckets; i++) {
       bucket_cnts[i].packet_count = htonll(gs->bucket_stats[i].packet_count);
       bucket_cnts[i].byte_count = htonll(gs->bucket_stats[i].byte_count);
    }
}
