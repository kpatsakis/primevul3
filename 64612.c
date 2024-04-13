ofputil_packet_in_reason_to_string(enum ofp_packet_in_reason reason,
                                   char *reasonbuf, size_t bufsize)
{
    switch (reason) {
    case OFPR_NO_MATCH:
        return "no_match";
    case OFPR_ACTION:
        return "action";
    case OFPR_INVALID_TTL:
        return "invalid_ttl";
    case OFPR_ACTION_SET:
        return "action_set";
    case OFPR_GROUP:
        return "group";
    case OFPR_PACKET_OUT:
        return "packet_out";
    case OFPR_EXPLICIT_MISS:
    case OFPR_IMPLICIT_MISS:
        return "";

    case OFPR_N_REASONS:
    default:
        snprintf(reasonbuf, bufsize, "%d", (int) reason);
        return reasonbuf;
    }
}
