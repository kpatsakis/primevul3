ofputil_decode_switch_config(const struct ofp_header *oh,
                             struct ofputil_switch_config *config)
{
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    ofpraw_pull_assert(&b);

    const struct ofp_switch_config *osc = ofpbuf_pull(&b, sizeof *osc);
    config->frag = ntohs(osc->flags) & OFPC_FRAG_MASK;
    config->miss_send_len = ntohs(osc->miss_send_len);

    ovs_be16 valid_mask = htons(OFPC_FRAG_MASK);
    if (oh->version < OFP13_VERSION) {
        const ovs_be16 ttl_bit = htons(OFPC_INVALID_TTL_TO_CONTROLLER);
        valid_mask |= ttl_bit;
        config->invalid_ttl_to_controller = (osc->flags & ttl_bit) != 0;
    } else {
        config->invalid_ttl_to_controller = -1;
    }

    return !(osc->flags & ~valid_mask);
}
