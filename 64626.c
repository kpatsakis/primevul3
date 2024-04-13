ofputil_protocol_set_base(enum ofputil_protocol cur,
                          enum ofputil_protocol new_base)
{
    bool tid = (cur & OFPUTIL_P_TID) != 0;

    switch (new_base) {
    case OFPUTIL_P_OF10_STD:
    case OFPUTIL_P_OF10_STD_TID:
        return ofputil_protocol_set_tid(OFPUTIL_P_OF10_STD, tid);

    case OFPUTIL_P_OF10_NXM:
    case OFPUTIL_P_OF10_NXM_TID:
        return ofputil_protocol_set_tid(OFPUTIL_P_OF10_NXM, tid);

    case OFPUTIL_P_OF11_STD:
        return ofputil_protocol_set_tid(OFPUTIL_P_OF11_STD, tid);

    case OFPUTIL_P_OF12_OXM:
        return ofputil_protocol_set_tid(OFPUTIL_P_OF12_OXM, tid);

    case OFPUTIL_P_OF13_OXM:
        return ofputil_protocol_set_tid(OFPUTIL_P_OF13_OXM, tid);

    case OFPUTIL_P_OF14_OXM:
        return ofputil_protocol_set_tid(OFPUTIL_P_OF14_OXM, tid);

    case OFPUTIL_P_OF15_OXM:
        return ofputil_protocol_set_tid(OFPUTIL_P_OF15_OXM, tid);

    case OFPUTIL_P_OF16_OXM:
        return ofputil_protocol_set_tid(OFPUTIL_P_OF16_OXM, tid);

    default:
        OVS_NOT_REACHED();
    }
}
