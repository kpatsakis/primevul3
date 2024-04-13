ofputil_decode_table_miss(ovs_be32 config_, enum ofp_version version)
{
    uint32_t config = ntohl(config_);

    if (version == OFP11_VERSION || version == OFP12_VERSION) {
        switch (config & OFPTC11_TABLE_MISS_MASK) {
        case OFPTC11_TABLE_MISS_CONTROLLER:
            return OFPUTIL_TABLE_MISS_CONTROLLER;

        case OFPTC11_TABLE_MISS_CONTINUE:
            return OFPUTIL_TABLE_MISS_CONTINUE;

        case OFPTC11_TABLE_MISS_DROP:
            return OFPUTIL_TABLE_MISS_DROP;

        default:
            VLOG_WARN_RL(&bad_ofmsg_rl, "bad table miss config %d", config);
            return OFPUTIL_TABLE_MISS_CONTROLLER;
        }
    } else {
        return OFPUTIL_TABLE_MISS_DEFAULT;
    }
}
