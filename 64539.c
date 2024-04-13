ofputil_encode_group_desc_request(enum ofp_version ofp_version,
                                  uint32_t group_id)
{
    struct ofpbuf *request;

    switch (ofp_version) {
    case OFP10_VERSION:
        ovs_fatal(0, "dump-groups needs OpenFlow 1.1 or later "
                     "(\'-O OpenFlow11\')");
    case OFP11_VERSION:
    case OFP12_VERSION:
    case OFP13_VERSION:
    case OFP14_VERSION:
        request = ofpraw_alloc(OFPRAW_OFPST11_GROUP_DESC_REQUEST,
                               ofp_version, 0);
        break;
    case OFP15_VERSION:
    case OFP16_VERSION: {
        struct ofp15_group_desc_request *req;
        request = ofpraw_alloc(OFPRAW_OFPST15_GROUP_DESC_REQUEST,
                               ofp_version, 0);
        req = ofpbuf_put_zeros(request, sizeof *req);
        req->group_id = htonl(group_id);
        break;
    }
    default:
        OVS_NOT_REACHED();
    }

    return request;
}
