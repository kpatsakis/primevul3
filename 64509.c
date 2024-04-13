ofputil_decode_requestforward(const struct ofp_header *outer,
                              struct ofputil_requestforward *rf)
{
    struct ofpbuf b = ofpbuf_const_initializer(outer, ntohs(outer->length));

    /* Skip past outer message. */
    enum ofpraw outer_raw = ofpraw_pull_assert(&b);
    ovs_assert(outer_raw == OFPRAW_OFPT14_REQUESTFORWARD);

    /* Validate inner message. */
    if (b.size < sizeof(struct ofp_header)) {
        return OFPERR_OFPBFC_MSG_BAD_LEN;
    }
    const struct ofp_header *inner = b.data;
    unsigned int inner_len = ntohs(inner->length);
    if (inner_len < sizeof(struct ofp_header) || inner_len > b.size) {
        return OFPERR_OFPBFC_MSG_BAD_LEN;
    }
    if (inner->version != outer->version) {
        return OFPERR_OFPBRC_BAD_VERSION;
    }

    /* Parse inner message. */
    enum ofptype type;
    enum ofperr error = ofptype_decode(&type, inner);
    if (error) {
        return error;
    }

    rf->xid = inner->xid;
    if (type == OFPTYPE_GROUP_MOD) {
        rf->reason = OFPRFR_GROUP_MOD;
        rf->group_mod = xmalloc(sizeof *rf->group_mod);
        error = ofputil_decode_group_mod(inner, rf->group_mod);
        if (error) {
            free(rf->group_mod);
            return error;
        }
    } else if (type == OFPTYPE_METER_MOD) {
        rf->reason = OFPRFR_METER_MOD;
        rf->meter_mod = xmalloc(sizeof *rf->meter_mod);
        ofpbuf_init(&rf->bands, 64);
        error = ofputil_decode_meter_mod(inner, rf->meter_mod, &rf->bands);
        if (error) {
            free(rf->meter_mod);
            ofpbuf_uninit(&rf->bands);
            return error;
        }
    } else {
        return OFPERR_OFPBFC_MSG_UNSUP;
    }

    return 0;
}
