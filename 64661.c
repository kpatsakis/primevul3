ofputil_put_packet_in_private(const struct ofputil_packet_in_private *pin,
                              enum ofp_version version, size_t include_bytes,
                              struct ofpbuf *msg)
{
    ofputil_put_packet_in(&pin->public, version, include_bytes, msg);

    size_t continuation_ofs = ofpprop_start_nested(msg, NXPINT_CONTINUATION);
    size_t inner_ofs = msg->size;

    if (!uuid_is_zero(&pin->bridge)) {
        ofpprop_put_uuid(msg, NXCPT_BRIDGE, &pin->bridge);
    }

    struct ofpbuf pin_stack;
    ofpbuf_use_const(&pin_stack, pin->stack, pin->stack_size);

    while (pin_stack.size) {
        uint8_t len;
        uint8_t *val = nx_stack_pop(&pin_stack, &len);
        ofpprop_put(msg, NXCPT_STACK, val, len);
    }

    if (pin->mirrors) {
        ofpprop_put_u32(msg, NXCPT_MIRRORS, pin->mirrors);
    }

    if (pin->conntracked) {
        ofpprop_put_flag(msg, NXCPT_CONNTRACKED);
    }

    if (pin->actions_len) {
        /* Divide 'pin->actions' into groups that begins with an
         * unroll_xlate action.  For each group, emit a NXCPT_TABLE_ID and
         * NXCPT_COOKIE property (if either has changed; each is initially
         * assumed 0), then a NXCPT_ACTIONS property with the grouped
         * actions.
         *
         * The alternative is to make OFPACT_UNROLL_XLATE public.  We can
         * always do that later, since this is a private property. */
        const struct ofpact *const end = ofpact_end(pin->actions,
                                                    pin->actions_len);
        const struct ofpact_unroll_xlate *unroll = NULL;
        uint8_t table_id = 0;
        ovs_be64 cookie = 0;

        const struct ofpact *a;
        for (a = pin->actions; ; a = ofpact_next(a)) {
            if (a == end || a->type == OFPACT_UNROLL_XLATE) {
                if (unroll) {
                    if (table_id != unroll->rule_table_id) {
                        ofpprop_put_u8(msg, NXCPT_TABLE_ID,
                                       unroll->rule_table_id);
                        table_id = unroll->rule_table_id;
                    }
                    if (cookie != unroll->rule_cookie) {
                        ofpprop_put_be64(msg, NXCPT_COOKIE,
                                         unroll->rule_cookie);
                        cookie = unroll->rule_cookie;
                    }
                }

                const struct ofpact *start
                    = unroll ? ofpact_next(&unroll->ofpact) : pin->actions;
                put_actions_property(msg, NXCPT_ACTIONS, version,
                                     start, (a - start) * sizeof *a);

                if (a == end) {
                    break;
                }
                unroll = ofpact_get_UNROLL_XLATE(a);
            }
        }
    }

    if (pin->action_set_len) {
        size_t start = ofpprop_start_nested(msg, NXCPT_ACTION_SET);
        ofpacts_put_openflow_actions(pin->action_set,
                                     pin->action_set_len, msg, version);
        ofpprop_end(msg, start);
    }

    if (msg->size > inner_ofs) {
        ofpprop_end(msg, continuation_ofs);
    } else {
        msg->size = continuation_ofs;
    }
}
