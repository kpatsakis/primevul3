ofputil_match_typical_len(enum ofputil_protocol protocol)
{
    switch (protocol) {
    case OFPUTIL_P_OF10_STD:
    case OFPUTIL_P_OF10_STD_TID:
        return sizeof(struct ofp10_match);

    case OFPUTIL_P_OF10_NXM:
    case OFPUTIL_P_OF10_NXM_TID:
        return NXM_TYPICAL_LEN;

    case OFPUTIL_P_OF11_STD:
        return sizeof(struct ofp11_match);

    case OFPUTIL_P_OF12_OXM:
    case OFPUTIL_P_OF13_OXM:
    case OFPUTIL_P_OF14_OXM:
    case OFPUTIL_P_OF15_OXM:
    case OFPUTIL_P_OF16_OXM:
        return NXM_TYPICAL_LEN;

    default:
        OVS_NOT_REACHED();
    }
}
