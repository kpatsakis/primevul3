parse_async_tlv(const struct ofpbuf *property,
                const struct ofp14_async_prop *ap,
                struct ofputil_async_cfg *ac,
                enum ofp_version version, bool loose)
{
    enum ofperr error;
    ovs_be32 mask;

    error  = ofpprop_parse_be32(property, &mask);
    if (error) {
        return error;
    }

    if (ofpprop_is_experimenter(ap->prop_type)) {
        /* For experimenter properties, whether a property is for the master or
         * slave role is indicated by both 'type' and 'exp_type' in struct
         * ofp_prop_experimenter.  Check that these are consistent. */
        const struct ofp_prop_experimenter *ope = property->data;
        bool should_be_master = ope->type == htons(0xffff);
        if (should_be_master != ap->master) {
            VLOG_WARN_RL(&bad_ofmsg_rl, "async property type %#"PRIx16" "
                         "indicates %s role but exp_type %"PRIu32" indicates "
                         "%s role",
                         ntohs(ope->type),
                         should_be_master ? "master" : "slave",
                         ntohl(ope->exp_type),
                         ap->master ? "master" : "slave");
            return OFPERR_OFPBPC_BAD_EXP_TYPE;
        }
    }

    return decode_async_mask(mask, ap, version, loose, ac);
}
