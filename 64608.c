ofputil_packet_in_format_is_valid(enum nx_packet_in_format packet_in_format)
{
    switch (packet_in_format) {
    case NXPIF_STANDARD:
    case NXPIF_NXT_PACKET_IN:
    case NXPIF_NXT_PACKET_IN2:
        return true;
    }

    return false;
}
