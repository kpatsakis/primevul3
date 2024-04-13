ofputil_encode_port_desc_stats_request(enum ofp_version ofp_version,
                                       ofp_port_t port)
{
    struct ofpbuf *request;

    switch (ofp_version) {
    case OFP10_VERSION:
    case OFP11_VERSION:
    case OFP12_VERSION:
    case OFP13_VERSION:
    case OFP14_VERSION:
        request = ofpraw_alloc(OFPRAW_OFPST10_PORT_DESC_REQUEST,
                               ofp_version, 0);
        break;
    case OFP15_VERSION:
    case OFP16_VERSION:{
        struct ofp15_port_desc_request *req;
        request = ofpraw_alloc(OFPRAW_OFPST15_PORT_DESC_REQUEST,
                               ofp_version, 0);
        req = ofpbuf_put_zeros(request, sizeof *req);
        req->port_no = ofputil_port_to_ofp11(port);
        break;
    }
    default:
        OVS_NOT_REACHED();
    }

    return request;
}
