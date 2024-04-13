ofputil_packet_in_reason_from_string(const char *s,
                                     enum ofp_packet_in_reason *reason)
{
    int i;

    for (i = 0; i < OFPR_N_REASONS; i++) {
        char reasonbuf[OFPUTIL_PACKET_IN_REASON_BUFSIZE];
        const char *reason_s;

        reason_s = ofputil_packet_in_reason_to_string(i, reasonbuf,
                                                      sizeof reasonbuf);
        if (!strcasecmp(s, reason_s)) {
            *reason = i;
            return true;
        }
    }
    return false;
}
