    OVS_REQUIRES(ofproto_mutex)
{
    struct ofputil_group_stats ogs;
    const struct ofproto *ofproto = group->ofproto;
    long long int now = time_msec();
    int error;

    ogs.bucket_stats = xmalloc(group->n_buckets * sizeof *ogs.bucket_stats);

    /* Provider sets the packet and byte counts, we do the rest. */
    ogs.ref_count = rule_collection_n(&group->rules);
    ogs.n_buckets = group->n_buckets;

    error = (ofproto->ofproto_class->group_get_stats
             ? ofproto->ofproto_class->group_get_stats(group, &ogs)
             : EOPNOTSUPP);
    if (error) {
        ogs.packet_count = UINT64_MAX;
        ogs.byte_count = UINT64_MAX;
        memset(ogs.bucket_stats, 0xff,
               ogs.n_buckets * sizeof *ogs.bucket_stats);
    }

    ogs.group_id = group->group_id;
    calc_duration(group->created, now, &ogs.duration_sec, &ogs.duration_nsec);

    ofputil_append_group_stats(replies, &ogs);

    free(ogs.bucket_stats);
}
