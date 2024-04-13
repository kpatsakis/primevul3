ofputil_protocols_from_ofp_version(enum ofp_version version)
{
    switch (version) {
    case OFP10_VERSION:
        return OFPUTIL_P_OF10_STD_ANY | OFPUTIL_P_OF10_NXM_ANY;
    case OFP11_VERSION:
        return OFPUTIL_P_OF11_STD;
    case OFP12_VERSION:
        return OFPUTIL_P_OF12_OXM;
    case OFP13_VERSION:
        return OFPUTIL_P_OF13_OXM;
    case OFP14_VERSION:
        return OFPUTIL_P_OF14_OXM;
    case OFP15_VERSION:
        return OFPUTIL_P_OF15_OXM;
    case OFP16_VERSION:
        return OFPUTIL_P_OF16_OXM;
    default:
        return 0;
    }
}
