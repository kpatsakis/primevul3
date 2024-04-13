ofputil_append_queue_stat(struct ovs_list *replies,
                          const struct ofputil_queue_stats *oqs)
{
    switch (ofpmp_version(replies)) {
    case OFP13_VERSION: {
        struct ofp13_queue_stats *reply = ofpmp_append(replies, sizeof *reply);
        ofputil_queue_stats_to_ofp13(oqs, reply);
        break;
    }

    case OFP12_VERSION:
    case OFP11_VERSION: {
        struct ofp11_queue_stats *reply = ofpmp_append(replies, sizeof *reply);
        ofputil_queue_stats_to_ofp11(oqs, reply);
        break;
    }

    case OFP10_VERSION: {
        struct ofp10_queue_stats *reply = ofpmp_append(replies, sizeof *reply);
        ofputil_queue_stats_to_ofp10(oqs, reply);
        break;
    }

    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION: {
        struct ofp14_queue_stats *reply = ofpmp_append(replies, sizeof *reply);
        ofputil_queue_stats_to_ofp14(oqs, reply);
        break;
    }

    default:
        OVS_NOT_REACHED();
    }
}
