ofputil_decode_port_stats_request(const struct ofp_header *request,
                                  ofp_port_t *ofp10_port)
{
    switch ((enum ofp_version)request->version) {
    case OFP16_VERSION:
    case OFP15_VERSION:
    case OFP14_VERSION:
    case OFP13_VERSION:
    case OFP12_VERSION:
    case OFP11_VERSION: {
        const struct ofp11_port_stats_request *psr11 = ofpmsg_body(request);
        return ofputil_port_from_ofp11(psr11->port_no, ofp10_port);
    }

    case OFP10_VERSION: {
        const struct ofp10_port_stats_request *psr10 = ofpmsg_body(request);
        *ofp10_port = u16_to_ofp(ntohs(psr10->port_no));
        return 0;
    }

    default:
        OVS_NOT_REACHED();
    }
}
