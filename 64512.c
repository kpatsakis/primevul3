ofputil_decode_set_async_config(const struct ofp_header *oh, bool loose,
                                const struct ofputil_async_cfg *basis,
                                struct ofputil_async_cfg *ac)
{
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    enum ofpraw raw = ofpraw_pull_assert(&b);

    if (raw == OFPRAW_OFPT13_SET_ASYNC ||
        raw == OFPRAW_NXT_SET_ASYNC_CONFIG ||
        raw == OFPRAW_OFPT13_GET_ASYNC_REPLY) {
        const struct nx_async_config *msg = ofpmsg_body(oh);

        *ac = OFPUTIL_ASYNC_CFG_INIT;
        decode_legacy_async_masks(msg->packet_in_mask, OAM_PACKET_IN,
                                  oh->version, ac);
        decode_legacy_async_masks(msg->port_status_mask, OAM_PORT_STATUS,
                                  oh->version, ac);
        decode_legacy_async_masks(msg->flow_removed_mask, OAM_FLOW_REMOVED,
                                  oh->version, ac);
    } else if (raw == OFPRAW_OFPT14_SET_ASYNC ||
               raw == OFPRAW_OFPT14_GET_ASYNC_REPLY ||
               raw == OFPRAW_NXT_SET_ASYNC_CONFIG2) {
        *ac = *basis;
        while (b.size > 0) {
            struct ofpbuf property;
            enum ofperr error;
            uint64_t type;

            error = ofpprop_pull__(&b, &property, 8, 0xfffe, &type);
            if (error) {
                return error;
            }

            const struct ofp14_async_prop *ap
                = get_ofp14_async_config_prop_by_prop_type(type);
            error = (ap
                     ? parse_async_tlv(&property, ap, ac, oh->version, loose)
                     : OFPPROP_UNKNOWN(loose, "async config", type));
            if (error) {
                /* Most messages use OFPBPC_BAD_TYPE but async has its own (who
                 * knows why, it's OpenFlow. */
                if (error == OFPERR_OFPBPC_BAD_TYPE) {
                    error = OFPERR_OFPACFC_UNSUPPORTED;
                }
                return error;
            }
        }
    } else {
        return OFPERR_OFPBRC_BAD_VERSION;
    }
    return 0;
}
