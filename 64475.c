ofputil_decode_get_config_reply(const struct ofp_header *oh,
                                struct ofputil_switch_config *config)
{
    ofputil_decode_switch_config(oh, config);
}
