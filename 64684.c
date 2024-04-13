parse_intel_port_stats_property(const struct ofpbuf *payload,
                                uint32_t exp_type,
                                struct ofputil_port_stats *ops)
{
    enum ofperr error;

    switch (exp_type) {
    case INTEL_PORT_STATS_RFC2819:
        error = parse_intel_port_stats_rfc2819_property(payload, ops);
        break;
    default:
        error = OFPERR_OFPBPC_BAD_EXP_TYPE;
        break;
    }

    return error;
}
