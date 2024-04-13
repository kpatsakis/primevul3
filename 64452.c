ofputil_append_table_stats_reply(struct ofpbuf *reply,
                                 const struct ofputil_table_stats *stats,
                                 const struct ofputil_table_features *features)
{
    struct ofp_header *oh = reply->header;

    ovs_assert(stats->table_id == features->table_id);

    switch ((enum ofp_version) oh->version) {
    case OFP10_VERSION:
        ofputil_put_ofp10_table_stats(stats, features, reply);
        break;

    case OFP11_VERSION:
        ofputil_put_ofp11_table_stats(stats, features, reply);
        break;

    case OFP12_VERSION:
        ofputil_put_ofp12_table_stats(stats, features, reply);
        break;

    case OFP13_VERSION:
    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION:
        ofputil_put_ofp13_table_stats(stats, reply);
        break;

    default:
        OVS_NOT_REACHED();
    }
}
