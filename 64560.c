ofputil_encode_port_status(const struct ofputil_port_status *ps,
                           enum ofputil_protocol protocol)
{
    struct ofp_port_status *ops;
    struct ofpbuf *b;
    enum ofp_version version;
    enum ofpraw raw;

    version = ofputil_protocol_to_ofp_version(protocol);
    switch (version) {
    case OFP10_VERSION:
        raw = OFPRAW_OFPT10_PORT_STATUS;
        break;

    case OFP11_VERSION:
    case OFP12_VERSION:
    case OFP13_VERSION:
        raw = OFPRAW_OFPT11_PORT_STATUS;
        break;

    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION:
        raw = OFPRAW_OFPT14_PORT_STATUS;
        break;

    default:
        OVS_NOT_REACHED();
    }

    b = ofpraw_alloc_xid(raw, version, htonl(0), 0);
    ops = ofpbuf_put_zeros(b, sizeof *ops);
    ops->reason = ps->reason;
    ofputil_put_phy_port(version, &ps->desc, b);
    ofpmsg_update_length(b);
    return b;
}
