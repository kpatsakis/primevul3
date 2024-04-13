ofputil_put_switch_config(const struct ofputil_switch_config *config,
                          struct ofpbuf *b)
{
    const struct ofp_header *oh = b->data;
    struct ofp_switch_config *osc = ofpbuf_put_zeros(b, sizeof *osc);
    osc->flags = htons(config->frag);
    if (config->invalid_ttl_to_controller > 0 && oh->version < OFP13_VERSION) {
        osc->flags |= htons(OFPC_INVALID_TTL_TO_CONTROLLER);
    }
    osc->miss_send_len = htons(config->miss_send_len);
    return b;
}
