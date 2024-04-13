ofputil_decode_packet_in_private(const struct ofp_header *oh, bool loose,
                                 const struct tun_table *tun_table,
                                 const struct vl_mff_map *vl_mff_map,
                                 struct ofputil_packet_in_private *pin,
                                 size_t *total_len, uint32_t *buffer_id)
{
    memset(pin, 0, sizeof *pin);

    struct ofpbuf continuation;
    enum ofperr error;
    error = ofputil_decode_packet_in(oh, loose, tun_table, vl_mff_map,
                                     &pin->public, total_len, buffer_id,
                                     &continuation);
    if (error) {
        return error;
    }

    struct ofpbuf actions, action_set;
    ofpbuf_init(&actions, 0);
    ofpbuf_init(&action_set, 0);

    uint8_t table_id = 0;
    ovs_be64 cookie = 0;

    struct ofpbuf stack;
    ofpbuf_init(&stack, 0);

    while (continuation.size > 0) {
        struct ofpbuf payload;
        uint64_t type;

        error = ofpprop_pull(&continuation, &payload, &type);
        if (error) {
            break;
        }

        switch (type) {
        case NXCPT_BRIDGE:
            error = ofpprop_parse_uuid(&payload, &pin->bridge);
            break;

        case NXCPT_STACK:
            error = parse_stack_prop(&payload, &stack);
            break;

        case NXCPT_MIRRORS:
            error = ofpprop_parse_u32(&payload, &pin->mirrors);
            break;

        case NXCPT_CONNTRACKED:
            pin->conntracked = true;
            break;

        case NXCPT_TABLE_ID:
            error = ofpprop_parse_u8(&payload, &table_id);
            break;

        case NXCPT_COOKIE:
            error = ofpprop_parse_be64(&payload, &cookie);
            break;

        case NXCPT_ACTIONS: {
            struct ofpact_unroll_xlate *unroll
                = ofpact_put_UNROLL_XLATE(&actions);
            unroll->rule_table_id = table_id;
            unroll->rule_cookie = cookie;
            error = parse_actions_property(&payload, oh->version, &actions);
            break;
        }

        case NXCPT_ACTION_SET:
            error = parse_actions_property(&payload, oh->version, &action_set);
            break;

        default:
            error = OFPPROP_UNKNOWN(loose, "continuation", type);
            break;
        }
        if (error) {
            break;
        }
    }

    pin->actions_len = actions.size;
    pin->actions = ofpbuf_steal_data(&actions);
    pin->action_set_len = action_set.size;
    pin->action_set = ofpbuf_steal_data(&action_set);
    pin->stack_size = stack.size;
    pin->stack = ofpbuf_steal_data(&stack);

    if (error) {
        ofputil_packet_in_private_destroy(pin);
    }

    return error;
}
