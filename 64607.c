ofputil_packet_in_format_from_string(const char *s)
{
    return (!strcmp(s, "standard") || !strcmp(s, "openflow10")
            ? NXPIF_STANDARD
            : !strcmp(s, "nxt_packet_in") || !strcmp(s, "nxm")
            ? NXPIF_NXT_PACKET_IN
            : !strcmp(s, "nxt_packet_in2")
            ? NXPIF_NXT_PACKET_IN2
            : -1);
}
