ofputil_append_group_desc_reply(const struct ofputil_group_desc *gds,
                                const struct ovs_list *buckets,
                                struct ovs_list *replies)
{
    enum ofp_version version = ofpmp_version(replies);

    switch (version)
    {
    case OFP11_VERSION:
    case OFP12_VERSION:
    case OFP13_VERSION:
    case OFP14_VERSION:
        ofputil_append_ofp11_group_desc_reply(gds, buckets, replies, version);
        break;

    case OFP15_VERSION:
    case OFP16_VERSION:
        ofputil_append_ofp15_group_desc_reply(gds, buckets, replies, version);
        break;

    case OFP10_VERSION:
    default:
        OVS_NOT_REACHED();
    }
}
