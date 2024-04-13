ofputil_encode_flow_mod_flags(enum ofputil_flow_mod_flags flags,
                              enum ofp_version version)
{
    const struct ofputil_flow_mod_flag *f;
    uint16_t raw_flags;

    raw_flags = 0;
    for (f = ofputil_flow_mod_flags; f->raw_flag; f++) {
        if (f->flag & flags
            && version >= f->min_version
            && (!f->max_version || version <= f->max_version)) {
            raw_flags |= f->raw_flag;
        }
    }

    return htons(raw_flags);
}
