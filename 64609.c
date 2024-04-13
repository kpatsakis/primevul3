ofputil_packet_in_format_to_string(enum nx_packet_in_format packet_in_format)
{
    switch (packet_in_format) {
    case NXPIF_STANDARD:
        return "standard";
    case NXPIF_NXT_PACKET_IN:
        return "nxt_packet_in";
    case NXPIF_NXT_PACKET_IN2:
        return "nxt_packet_in2";
    default:
        OVS_NOT_REACHED();
    }
}
