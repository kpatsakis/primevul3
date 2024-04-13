ofputil_append_group_stats(struct ovs_list *replies,
                           const struct ofputil_group_stats *gs)
{
    size_t bucket_counter_size;
    struct ofp11_bucket_counter *bucket_counters;
    size_t length;

    bucket_counter_size = gs->n_buckets * sizeof(struct ofp11_bucket_counter);

    switch (ofpmp_version(replies)) {
    case OFP11_VERSION:
    case OFP12_VERSION:{
            struct ofp11_group_stats *gs11;

            length = sizeof *gs11 + bucket_counter_size;
            gs11 = ofpmp_append(replies, length);
            bucket_counters = (struct ofp11_bucket_counter *)(gs11 + 1);
            ofputil_group_stats_to_ofp11(gs, gs11, length, bucket_counters);
            break;
        }

    case OFP13_VERSION:
    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION: {
            struct ofp13_group_stats *gs13;

            length = sizeof *gs13 + bucket_counter_size;
            gs13 = ofpmp_append(replies, length);
            bucket_counters = (struct ofp11_bucket_counter *)(gs13 + 1);
            ofputil_group_stats_to_ofp13(gs, gs13, length, bucket_counters);
            break;
        }

    case OFP10_VERSION:
    default:
        OVS_NOT_REACHED();
    }
}
