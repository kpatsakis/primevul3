ofputil_encode_barrier_request(enum ofp_version ofp_version)
{
    enum ofpraw type;

    switch (ofp_version) {
    case OFP16_VERSION:
    case OFP15_VERSION:
    case OFP14_VERSION:
    case OFP13_VERSION:
    case OFP12_VERSION:
    case OFP11_VERSION:
        type = OFPRAW_OFPT11_BARRIER_REQUEST;
        break;

    case OFP10_VERSION:
        type = OFPRAW_OFPT10_BARRIER_REQUEST;
        break;

    default:
        OVS_NOT_REACHED();
    }

    return ofpraw_alloc(type, ofp_version, 0);
}
