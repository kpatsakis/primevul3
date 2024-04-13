ofputil_protocol_to_ofp_version(enum ofputil_protocol protocol)
{
    switch (protocol) {
    case OFPUTIL_P_OF10_STD:
    case OFPUTIL_P_OF10_STD_TID:
    case OFPUTIL_P_OF10_NXM:
    case OFPUTIL_P_OF10_NXM_TID:
        return OFP10_VERSION;
    case OFPUTIL_P_OF11_STD:
        return OFP11_VERSION;
    case OFPUTIL_P_OF12_OXM:
        return OFP12_VERSION;
    case OFPUTIL_P_OF13_OXM:
        return OFP13_VERSION;
    case OFPUTIL_P_OF14_OXM:
        return OFP14_VERSION;
    case OFPUTIL_P_OF15_OXM:
        return OFP15_VERSION;
    case OFPUTIL_P_OF16_OXM:
        return OFP16_VERSION;
    }

    OVS_NOT_REACHED();
}
