ofputil_encode_flow_mod(const struct ofputil_flow_mod *fm,
                        enum ofputil_protocol protocol)
{
    enum ofp_version version = ofputil_protocol_to_ofp_version(protocol);
    ovs_be16 raw_flags = ofputil_encode_flow_mod_flags(fm->flags, version);
    struct ofpbuf *msg;

    switch (protocol) {
    case OFPUTIL_P_OF11_STD:
    case OFPUTIL_P_OF12_OXM:
    case OFPUTIL_P_OF13_OXM:
    case OFPUTIL_P_OF14_OXM:
    case OFPUTIL_P_OF15_OXM:
    case OFPUTIL_P_OF16_OXM: {
        struct ofp11_flow_mod *ofm;
        int tailroom;

        tailroom = ofputil_match_typical_len(protocol) + fm->ofpacts_len;
        msg = ofpraw_alloc(OFPRAW_OFPT11_FLOW_MOD, version, tailroom);
        ofm = ofpbuf_put_zeros(msg, sizeof *ofm);
        if ((protocol == OFPUTIL_P_OF11_STD
             && (fm->command == OFPFC_MODIFY ||
                 fm->command == OFPFC_MODIFY_STRICT)
             && fm->cookie_mask == htonll(0))
            || fm->command == OFPFC_ADD) {
            ofm->cookie = fm->new_cookie;
        } else {
            ofm->cookie = fm->cookie & fm->cookie_mask;
        }
        ofm->cookie_mask = fm->cookie_mask;
        if (fm->table_id != OFPTT_ALL
            || (protocol != OFPUTIL_P_OF11_STD
                && (fm->command == OFPFC_DELETE ||
                    fm->command == OFPFC_DELETE_STRICT))) {
            ofm->table_id = fm->table_id;
        } else {
            ofm->table_id = 0;
        }
        ofm->command = fm->command;
        ofm->idle_timeout = htons(fm->idle_timeout);
        ofm->hard_timeout = htons(fm->hard_timeout);
        ofm->priority = htons(fm->priority);
        ofm->buffer_id = htonl(fm->buffer_id);
        ofm->out_port = ofputil_port_to_ofp11(fm->out_port);
        ofm->out_group = htonl(fm->out_group);
        ofm->flags = raw_flags;
        if (version >= OFP14_VERSION && fm->command == OFPFC_ADD) {
            ofm->importance = htons(fm->importance);
        } else {
            ofm->importance = 0;
        }
        ofputil_put_ofp11_match(msg, &fm->match, protocol);
        ofpacts_put_openflow_instructions(fm->ofpacts, fm->ofpacts_len, msg,
                                          version);
        break;
    }

    case OFPUTIL_P_OF10_STD:
    case OFPUTIL_P_OF10_STD_TID: {
        struct ofp10_flow_mod *ofm;

        msg = ofpraw_alloc(OFPRAW_OFPT10_FLOW_MOD, OFP10_VERSION,
                           fm->ofpacts_len);
        ofm = ofpbuf_put_zeros(msg, sizeof *ofm);
        ofputil_match_to_ofp10_match(&fm->match, &ofm->match);
        ofm->cookie = fm->new_cookie;
        ofm->command = ofputil_tid_command(fm, protocol);
        ofm->idle_timeout = htons(fm->idle_timeout);
        ofm->hard_timeout = htons(fm->hard_timeout);
        ofm->priority = htons(fm->priority);
        ofm->buffer_id = htonl(fm->buffer_id);
        ofm->out_port = htons(ofp_to_u16(fm->out_port));
        ofm->flags = raw_flags;
        ofpacts_put_openflow_actions(fm->ofpacts, fm->ofpacts_len, msg,
                                     version);
        break;
    }

    case OFPUTIL_P_OF10_NXM:
    case OFPUTIL_P_OF10_NXM_TID: {
        struct nx_flow_mod *nfm;
        int match_len;

        msg = ofpraw_alloc(OFPRAW_NXT_FLOW_MOD, OFP10_VERSION,
                           NXM_TYPICAL_LEN + fm->ofpacts_len);
        nfm = ofpbuf_put_zeros(msg, sizeof *nfm);
        nfm->command = ofputil_tid_command(fm, protocol);
        nfm->cookie = fm->new_cookie;
        match_len = nx_put_match(msg, &fm->match, fm->cookie, fm->cookie_mask);
        nfm = msg->msg;
        nfm->idle_timeout = htons(fm->idle_timeout);
        nfm->hard_timeout = htons(fm->hard_timeout);
        nfm->priority = htons(fm->priority);
        nfm->buffer_id = htonl(fm->buffer_id);
        nfm->out_port = htons(ofp_to_u16(fm->out_port));
        nfm->flags = raw_flags;
        nfm->match_len = htons(match_len);
        ofpacts_put_openflow_actions(fm->ofpacts, fm->ofpacts_len, msg,
                                     version);
        break;
    }

    default:
        OVS_NOT_REACHED();
    }

    ofpmsg_update_length(msg);
    return msg;
}
