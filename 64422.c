encode_packet_in_reason(enum ofp_packet_in_reason reason,
                        enum ofp_version version)
{
    switch (reason) {
    case OFPR_NO_MATCH:
    case OFPR_ACTION:
    case OFPR_INVALID_TTL:
        return reason;

    case OFPR_ACTION_SET:
    case OFPR_GROUP:
    case OFPR_PACKET_OUT:
        return version < OFP14_VERSION ? OFPR_ACTION : reason;

    case OFPR_EXPLICIT_MISS:
        return version < OFP13_VERSION ? OFPR_ACTION : OFPR_NO_MATCH;

    case OFPR_IMPLICIT_MISS:
        return OFPR_NO_MATCH;

    case OFPR_N_REASONS:
    default:
        OVS_NOT_REACHED();
    }
}
