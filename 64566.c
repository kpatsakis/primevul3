ofputil_encode_set_config(const struct ofputil_switch_config *config,
                          enum ofp_version version)
{
    struct ofpbuf *b = ofpraw_alloc(OFPRAW_OFPT_SET_CONFIG, version, 0);
    return ofputil_put_switch_config(config, b);
}
