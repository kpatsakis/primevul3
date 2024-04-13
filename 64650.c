ofputil_put_async_config__(const struct ofputil_async_cfg *ac,
                           struct ofpbuf *buf, bool tlv,
                           enum ofp_version version, uint32_t oams)
{
    if (!tlv) {
        struct nx_async_config *msg = ofpbuf_put_zeros(buf, sizeof *msg);
        encode_legacy_async_masks(ac, OAM_PACKET_IN, version,
                                  msg->packet_in_mask);
        encode_legacy_async_masks(ac, OAM_PORT_STATUS, version,
                                  msg->port_status_mask);
        encode_legacy_async_masks(ac, OAM_FLOW_REMOVED, version,
                                  msg->flow_removed_mask);
    } else {
        FOR_EACH_ASYNC_PROP (ap) {
            if (oams & (1u << ap->oam)) {
                size_t ofs = buf->size;
                ofpprop_put_be32(buf, ap->prop_type,
                                 encode_async_mask(ac, ap, version));

                /* For experimenter properties, we need to use type 0xfffe for
                 * master and 0xffff for slaves. */
                if (ofpprop_is_experimenter(ap->prop_type)) {
                    struct ofp_prop_experimenter *ope
                        = ofpbuf_at_assert(buf, ofs, sizeof *ope);
                    ope->type = ap->master ? htons(0xffff) : htons(0xfffe);
                }
            }
        }
    }
}
