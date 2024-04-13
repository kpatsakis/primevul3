ofputil_encode_bundle_ctrl_request(enum ofp_version ofp_version,
                                   struct ofputil_bundle_ctrl_msg *bc)
{
    struct ofpbuf *request;
    struct ofp14_bundle_ctrl_msg *m;

    switch (ofp_version) {
    case OFP10_VERSION:
    case OFP11_VERSION:
    case OFP12_VERSION:
        ovs_fatal(0, "bundles need OpenFlow 1.3 or later "
                     "(\'-O OpenFlow14\')");
    case OFP13_VERSION:
    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION:
        request = ofpraw_alloc(ofp_version == OFP13_VERSION
                               ? OFPRAW_ONFT13_BUNDLE_CONTROL
                               : OFPRAW_OFPT14_BUNDLE_CONTROL, ofp_version, 0);
        m = ofpbuf_put_zeros(request, sizeof *m);

        m->bundle_id = htonl(bc->bundle_id);
        m->type = htons(bc->type);
        m->flags = htons(bc->flags);
        break;
    default:
        OVS_NOT_REACHED();
    }

    return request;
}
