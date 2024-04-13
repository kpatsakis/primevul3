ofputil_encode_packet_in_private(const struct ofputil_packet_in_private *pin,
                                 enum ofputil_protocol protocol,
                                 enum nx_packet_in_format packet_in_format)
{
    enum ofp_version version = ofputil_protocol_to_ofp_version(protocol);

    struct ofpbuf *msg;
    switch (packet_in_format) {
    case NXPIF_STANDARD:
        switch (protocol) {
        case OFPUTIL_P_OF10_STD:
        case OFPUTIL_P_OF10_STD_TID:
        case OFPUTIL_P_OF10_NXM:
        case OFPUTIL_P_OF10_NXM_TID:
            msg = ofputil_encode_ofp10_packet_in(&pin->public);
            break;

        case OFPUTIL_P_OF11_STD:
            msg = ofputil_encode_ofp11_packet_in(&pin->public);
            break;

        case OFPUTIL_P_OF12_OXM:
        case OFPUTIL_P_OF13_OXM:
        case OFPUTIL_P_OF14_OXM:
        case OFPUTIL_P_OF15_OXM:
        case OFPUTIL_P_OF16_OXM:
            msg = ofputil_encode_ofp12_packet_in(&pin->public, version);
            break;

        default:
            OVS_NOT_REACHED();
        }
        break;

    case NXPIF_NXT_PACKET_IN:
        msg = ofputil_encode_nx_packet_in(&pin->public, version);
        break;

    case NXPIF_NXT_PACKET_IN2:
        return ofputil_encode_nx_packet_in2(pin, version,
                                            pin->public.packet_len);

    default:
        OVS_NOT_REACHED();
    }

    ofpbuf_put(msg, pin->public.packet, pin->public.packet_len);
    ofpmsg_update_length(msg);
    return msg;
}
