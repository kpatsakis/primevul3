ofputil_encode_queue_stats_request(enum ofp_version ofp_version,
                                   const struct ofputil_queue_stats_request *oqsr)
{
    struct ofpbuf *request;

    switch (ofp_version) {
    case OFP11_VERSION:
    case OFP12_VERSION:
    case OFP13_VERSION:
    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION: {
        struct ofp11_queue_stats_request *req;
        request = ofpraw_alloc(OFPRAW_OFPST11_QUEUE_REQUEST, ofp_version, 0);
        req = ofpbuf_put_zeros(request, sizeof *req);
        req->port_no = ofputil_port_to_ofp11(oqsr->port_no);
        req->queue_id = htonl(oqsr->queue_id);
        break;
    }
    case OFP10_VERSION: {
        struct ofp10_queue_stats_request *req;
        request = ofpraw_alloc(OFPRAW_OFPST10_QUEUE_REQUEST, ofp_version, 0);
        req = ofpbuf_put_zeros(request, sizeof *req);
        /* OpenFlow 1.0 needs OFPP_ALL instead of OFPP_ANY */
        req->port_no = htons(ofp_to_u16(oqsr->port_no == OFPP_ANY
                                        ? OFPP_ALL : oqsr->port_no));
        req->queue_id = htonl(oqsr->queue_id);
        break;
    }
    default:
        OVS_NOT_REACHED();
    }

    return request;
}
