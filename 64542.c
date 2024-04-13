ofputil_encode_group_stats_request(enum ofp_version ofp_version,
                                   uint32_t group_id)
{
    struct ofpbuf *request;

    switch (ofp_version) {
    case OFP10_VERSION:
        ovs_fatal(0, "dump-group-stats needs OpenFlow 1.1 or later "
                     "(\'-O OpenFlow11\')");
    case OFP11_VERSION:
    case OFP12_VERSION:
    case OFP13_VERSION:
    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION: {
        struct ofp11_group_stats_request *req;
        request = ofpraw_alloc(OFPRAW_OFPST11_GROUP_REQUEST, ofp_version, 0);
        req = ofpbuf_put_zeros(request, sizeof *req);
        req->group_id = htonl(group_id);
        break;
    }
    default:
        OVS_NOT_REACHED();
    }

    return request;
}
