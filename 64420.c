decode_async_mask(ovs_be32 src,
                  const struct ofp14_async_prop *ap, enum ofp_version version,
                  bool loose, struct ofputil_async_cfg *dst)
{
    uint32_t mask = ntohl(src);
    uint32_t allowed = ofp14_async_prop_allowed(ap, version);
    if (mask & ~allowed) {
        OFPPROP_LOG(&bad_ofmsg_rl, loose,
                    "bad value %#x for %s (allowed mask %#x)",
                    mask, ofputil_async_msg_type_to_string(ap->oam),
                    allowed);
        mask &= allowed;
        if (!loose) {
            return OFPERR_OFPACFC_INVALID;
        }
    }

    if (ap->oam == OAM_PACKET_IN) {
        if (mask & (1u << OFPR_NO_MATCH)) {
            mask |= 1u << OFPR_EXPLICIT_MISS;
            if (version < OFP13_VERSION) {
                mask |= 1u << OFPR_IMPLICIT_MISS;
            }
        }
    }

    uint32_t *array = ap->master ? dst->master : dst->slave;
    array[ap->oam] = mask;
    return 0;
}
