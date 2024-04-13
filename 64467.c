ofputil_decode_flow_mod(struct ofputil_flow_mod *fm,
                        const struct ofp_header *oh,
                        enum ofputil_protocol protocol,
                        const struct tun_table *tun_table,
                        const struct vl_mff_map *vl_mff_map,
                        struct ofpbuf *ofpacts,
                        ofp_port_t max_port, uint8_t max_table)
{
    ovs_be16 raw_flags;
    enum ofperr error;
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    enum ofpraw raw = ofpraw_pull_assert(&b);
    if (raw == OFPRAW_OFPT11_FLOW_MOD) {
        /* Standard OpenFlow 1.1+ flow_mod. */
        const struct ofp11_flow_mod *ofm;

        ofm = ofpbuf_pull(&b, sizeof *ofm);

        error = ofputil_pull_ofp11_match(&b, tun_table, vl_mff_map, &fm->match,
                                         NULL);
        if (error) {
            return error;
        }

        /* Translate the message. */
        fm->priority = ntohs(ofm->priority);
        if (ofm->command == OFPFC_ADD
            || (oh->version == OFP11_VERSION
                && (ofm->command == OFPFC_MODIFY ||
                    ofm->command == OFPFC_MODIFY_STRICT)
                && ofm->cookie_mask == htonll(0))) {
            /* In OpenFlow 1.1 only, a "modify" or "modify-strict" that does
             * not match on the cookie is treated as an "add" if there is no
             * match. */
            fm->cookie = htonll(0);
            fm->cookie_mask = htonll(0);
            fm->new_cookie = ofm->cookie;
        } else {
            fm->cookie = ofm->cookie;
            fm->cookie_mask = ofm->cookie_mask;
            fm->new_cookie = OVS_BE64_MAX;
        }
        fm->modify_cookie = false;
        fm->command = ofm->command;

        /* Get table ID.
         *
         * OF1.1 entirely forbids table_id == OFPTT_ALL.
         * OF1.2+ allows table_id == OFPTT_ALL only for deletes. */
        fm->table_id = ofm->table_id;
        if (fm->table_id == OFPTT_ALL
            && (oh->version == OFP11_VERSION
                || (ofm->command != OFPFC_DELETE &&
                    ofm->command != OFPFC_DELETE_STRICT))) {
            return OFPERR_OFPFMFC_BAD_TABLE_ID;
        }

        fm->idle_timeout = ntohs(ofm->idle_timeout);
        fm->hard_timeout = ntohs(ofm->hard_timeout);
        if (oh->version >= OFP14_VERSION && ofm->command == OFPFC_ADD) {
            fm->importance = ntohs(ofm->importance);
        } else {
            fm->importance = 0;
        }
        fm->buffer_id = ntohl(ofm->buffer_id);
        error = ofputil_port_from_ofp11(ofm->out_port, &fm->out_port);
        if (error) {
            return error;
        }

        fm->out_group = (ofm->command == OFPFC_DELETE ||
                         ofm->command == OFPFC_DELETE_STRICT
                         ? ntohl(ofm->out_group)
                         : OFPG_ANY);
        raw_flags = ofm->flags;
    } else {
        uint16_t command;

        if (raw == OFPRAW_OFPT10_FLOW_MOD) {
            /* Standard OpenFlow 1.0 flow_mod. */
            const struct ofp10_flow_mod *ofm;

            /* Get the ofp10_flow_mod. */
            ofm = ofpbuf_pull(&b, sizeof *ofm);

            /* Translate the rule. */
            ofputil_match_from_ofp10_match(&ofm->match, &fm->match);
            ofputil_normalize_match(&fm->match);

            /* OpenFlow 1.0 says that exact-match rules have to have the
             * highest possible priority. */
            fm->priority = (ofm->match.wildcards & htonl(OFPFW10_ALL)
                            ? ntohs(ofm->priority)
                            : UINT16_MAX);

            /* Translate the message. */
            command = ntohs(ofm->command);
            fm->cookie = htonll(0);
            fm->cookie_mask = htonll(0);
            fm->new_cookie = ofm->cookie;
            fm->idle_timeout = ntohs(ofm->idle_timeout);
            fm->hard_timeout = ntohs(ofm->hard_timeout);
            fm->importance = 0;
            fm->buffer_id = ntohl(ofm->buffer_id);
            fm->out_port = u16_to_ofp(ntohs(ofm->out_port));
            fm->out_group = OFPG_ANY;
            raw_flags = ofm->flags;
        } else if (raw == OFPRAW_NXT_FLOW_MOD) {
            /* Nicira extended flow_mod. */
            const struct nx_flow_mod *nfm;

            /* Dissect the message. */
            nfm = ofpbuf_pull(&b, sizeof *nfm);
            error = nx_pull_match(&b, ntohs(nfm->match_len),
                                  &fm->match, &fm->cookie, &fm->cookie_mask,
                                  tun_table, vl_mff_map);
            if (error) {
                return error;
            }

            /* Translate the message. */
            command = ntohs(nfm->command);
            if ((command & 0xff) == OFPFC_ADD && fm->cookie_mask) {
                /* Flow additions may only set a new cookie, not match an
                 * existing cookie. */
                return OFPERR_NXBRC_NXM_INVALID;
            }
            fm->priority = ntohs(nfm->priority);
            fm->new_cookie = nfm->cookie;
            fm->idle_timeout = ntohs(nfm->idle_timeout);
            fm->hard_timeout = ntohs(nfm->hard_timeout);
            fm->importance = 0;
            fm->buffer_id = ntohl(nfm->buffer_id);
            fm->out_port = u16_to_ofp(ntohs(nfm->out_port));
            fm->out_group = OFPG_ANY;
            raw_flags = nfm->flags;
        } else {
            OVS_NOT_REACHED();
        }

        fm->modify_cookie = fm->new_cookie != OVS_BE64_MAX;
        if (protocol & OFPUTIL_P_TID) {
            fm->command = command & 0xff;
            fm->table_id = command >> 8;
        } else {
            if (command > 0xff) {
                VLOG_WARN_RL(&bad_ofmsg_rl, "flow_mod has explicit table_id "
                             "but flow_mod_table_id extension is not enabled");
            }
            fm->command = command;
            fm->table_id = 0xff;
        }
    }

    if (fm->command > OFPFC_DELETE_STRICT) {
        return OFPERR_OFPFMFC_BAD_COMMAND;
    }

    fm->ofpacts_tlv_bitmap = 0;
    error = ofpacts_pull_openflow_instructions(&b, b.size, oh->version,
                                               vl_mff_map,
                                               &fm->ofpacts_tlv_bitmap,
                                               ofpacts);
    if (error) {
        return error;
    }
    fm->ofpacts = ofpacts->data;
    fm->ofpacts_len = ofpacts->size;

    error = ofputil_decode_flow_mod_flags(raw_flags, fm->command,
                                          oh->version, &fm->flags);
    if (error) {
        return error;
    }

    if (fm->flags & OFPUTIL_FF_EMERG) {
        /* We do not support the OpenFlow 1.0 emergency flow cache, which
         * is not required in OpenFlow 1.0.1 and removed from OpenFlow 1.1.
         *
         * OpenFlow 1.0 specifies the error code to use when idle_timeout
         * or hard_timeout is nonzero.  Otherwise, there is no good error
         * code, so just state that the flow table is full. */
        return (fm->hard_timeout || fm->idle_timeout
                ? OFPERR_OFPFMFC_BAD_EMERG_TIMEOUT
                : OFPERR_OFPFMFC_TABLE_FULL);
    }

    return ofpacts_check_consistency(fm->ofpacts, fm->ofpacts_len,
                                     &fm->match.flow, max_port,
                                     fm->table_id, max_table, protocol);
}
