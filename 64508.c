ofputil_decode_queue_stats_request(const struct ofp_header *request,
                                   struct ofputil_queue_stats_request *oqsr)
{
    switch ((enum ofp_version)request->version) {
    case OFP16_VERSION:
    case OFP15_VERSION:
    case OFP14_VERSION:
    case OFP13_VERSION:
    case OFP12_VERSION:
    case OFP11_VERSION: {
        const struct ofp11_queue_stats_request *qsr11 = ofpmsg_body(request);
        oqsr->queue_id = ntohl(qsr11->queue_id);
        return ofputil_port_from_ofp11(qsr11->port_no, &oqsr->port_no);
    }

    case OFP10_VERSION: {
        const struct ofp10_queue_stats_request *qsr10 = ofpmsg_body(request);
        oqsr->queue_id = ntohl(qsr10->queue_id);
        oqsr->port_no = u16_to_ofp(ntohs(qsr10->port_no));
        /* OF 1.0 uses OFPP_ALL for OFPP_ANY */
        if (oqsr->port_no == OFPP_ALL) {
            oqsr->port_no = OFPP_ANY;
        }
        return 0;
    }

    default:
        OVS_NOT_REACHED();
    }
}
