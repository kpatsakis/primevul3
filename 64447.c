ofputil_append_port_stat(struct ovs_list *replies,
                         const struct ofputil_port_stats *ops)
{
    switch (ofpmp_version(replies)) {
    case OFP13_VERSION: {
        struct ofp13_port_stats *reply = ofpmp_append(replies, sizeof *reply);
        ofputil_port_stats_to_ofp13(ops, reply);
        break;
    }
    case OFP12_VERSION:
    case OFP11_VERSION: {
        struct ofp11_port_stats *reply = ofpmp_append(replies, sizeof *reply);
        ofputil_port_stats_to_ofp11(ops, reply);
        break;
    }

    case OFP10_VERSION: {
        struct ofp10_port_stats *reply = ofpmp_append(replies, sizeof *reply);
        ofputil_port_stats_to_ofp10(ops, reply);
        break;
    }

    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION:
        ofputil_append_ofp14_port_stats(ops, replies);
        break;

    default:
        OVS_NOT_REACHED();
    }
}
