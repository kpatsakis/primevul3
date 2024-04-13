parse_ofp15_group_properties(struct ofpbuf *msg,
                             enum ofp11_group_type group_type,
                             enum ofp15_group_mod_command group_cmd,
                             struct ofputil_group_props *gp,
                             size_t properties_len)
{
    struct ofpbuf properties = ofpbuf_const_initializer(
        ofpbuf_pull(msg, properties_len), properties_len);
    while (properties.size > 0) {
        struct ofpbuf payload;
        enum ofperr error;
        uint64_t type;

        error = ofpprop_pull(&properties, &payload, &type);
        if (error) {
            return error;
        }

        switch (type) {
        case OFPPROP_EXP(NTR_VENDOR_ID, NTRT_SELECTION_METHOD):
        case OFPPROP_EXP(NTR_COMPAT_VENDOR_ID, NTRT_SELECTION_METHOD):
            error = parse_group_prop_ntr_selection_method(&payload, group_type,
                                                          group_cmd, gp);
            break;

        default:
            error = OFPPROP_UNKNOWN(false, "group", type);
            break;
        }

        if (error) {
            return error;
        }
    }

    return 0;
}
