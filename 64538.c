ofputil_encode_get_config_reply(const struct ofp_header *request,
                                const struct ofputil_switch_config *config)
{
    struct ofpbuf *b = ofpraw_alloc_reply(OFPRAW_OFPT_GET_CONFIG_REPLY,
                                          request, 0);
    return ofputil_put_switch_config(config, b);
}
