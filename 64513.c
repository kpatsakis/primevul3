ofputil_decode_set_config(const struct ofp_header *oh,
                          struct ofputil_switch_config *config)
{
    return (ofputil_decode_switch_config(oh, config)
            ? 0
            : OFPERR_OFPSCFC_BAD_FLAGS);
}
