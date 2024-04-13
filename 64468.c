ofputil_decode_flow_mod_flags(ovs_be16 raw_flags_,
                              enum ofp_flow_mod_command command,
                              enum ofp_version version,
                              enum ofputil_flow_mod_flags *flagsp)
{
    uint16_t raw_flags = ntohs(raw_flags_);
    const struct ofputil_flow_mod_flag *f;

    *flagsp = 0;
    for (f = ofputil_flow_mod_flags; f->raw_flag; f++) {
        if (raw_flags & f->raw_flag
            && version >= f->min_version
            && (!f->max_version || version <= f->max_version)) {
            raw_flags &= ~f->raw_flag;
            *flagsp |= f->flag;
        }
    }

    /* In OF1.0 and OF1.1, "add" always resets counters, and other commands
     * never do.
     *
     * In OF1.2 and later, OFPFF12_RESET_COUNTS controls whether each command
     * resets counters. */
    if ((version == OFP10_VERSION || version == OFP11_VERSION)
        && command == OFPFC_ADD) {
        *flagsp |= OFPUTIL_FF_RESET_COUNTS;
    }

    return raw_flags ? OFPERR_OFPFMFC_BAD_FLAGS : 0;
}
