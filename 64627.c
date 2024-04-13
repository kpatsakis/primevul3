ofputil_protocol_set_tid(enum ofputil_protocol protocol, bool enable)
{
    switch (protocol) {
    case OFPUTIL_P_OF10_STD:
    case OFPUTIL_P_OF10_STD_TID:
        return enable ? OFPUTIL_P_OF10_STD_TID : OFPUTIL_P_OF10_STD;

    case OFPUTIL_P_OF10_NXM:
    case OFPUTIL_P_OF10_NXM_TID:
        return enable ? OFPUTIL_P_OF10_NXM_TID : OFPUTIL_P_OF10_NXM;

    case OFPUTIL_P_OF11_STD:
        return OFPUTIL_P_OF11_STD;

    case OFPUTIL_P_OF12_OXM:
        return OFPUTIL_P_OF12_OXM;

    case OFPUTIL_P_OF13_OXM:
        return OFPUTIL_P_OF13_OXM;

    case OFPUTIL_P_OF14_OXM:
        return OFPUTIL_P_OF14_OXM;

    case OFPUTIL_P_OF15_OXM:
        return OFPUTIL_P_OF15_OXM;

    case OFPUTIL_P_OF16_OXM:
        return OFPUTIL_P_OF16_OXM;

    default:
        OVS_NOT_REACHED();
    }
}
