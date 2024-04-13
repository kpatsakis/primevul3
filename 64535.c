ofputil_encode_flow_removed(const struct ofputil_flow_removed *fr,
                            enum ofputil_protocol protocol)
{
    struct ofpbuf *msg;
    enum ofp_flow_removed_reason reason = fr->reason;

    if (reason == OFPRR_METER_DELETE && !(protocol & OFPUTIL_P_OF14_UP)) {
        reason = OFPRR_DELETE;
    }

    switch (protocol) {
    case OFPUTIL_P_OF11_STD:
    case OFPUTIL_P_OF12_OXM:
    case OFPUTIL_P_OF13_OXM:
    case OFPUTIL_P_OF14_OXM:
    case OFPUTIL_P_OF15_OXM:
    case OFPUTIL_P_OF16_OXM: {
        struct ofp12_flow_removed *ofr;

        msg = ofpraw_alloc_xid(OFPRAW_OFPT11_FLOW_REMOVED,
                               ofputil_protocol_to_ofp_version(protocol),
                               htonl(0),
                               ofputil_match_typical_len(protocol));
        ofr = ofpbuf_put_zeros(msg, sizeof *ofr);
        ofr->cookie = fr->cookie;
        ofr->priority = htons(fr->priority);
        ofr->reason = reason;
        ofr->table_id = fr->table_id;
        ofr->duration_sec = htonl(fr->duration_sec);
        ofr->duration_nsec = htonl(fr->duration_nsec);
        ofr->idle_timeout = htons(fr->idle_timeout);
        ofr->hard_timeout = htons(fr->hard_timeout);
        ofr->packet_count = htonll(fr->packet_count);
        ofr->byte_count = htonll(fr->byte_count);
        ofputil_put_ofp11_match(msg, &fr->match, protocol);
        break;
    }

    case OFPUTIL_P_OF10_STD:
    case OFPUTIL_P_OF10_STD_TID: {
        struct ofp10_flow_removed *ofr;

        msg = ofpraw_alloc_xid(OFPRAW_OFPT10_FLOW_REMOVED, OFP10_VERSION,
                               htonl(0), 0);
        ofr = ofpbuf_put_zeros(msg, sizeof *ofr);
        ofputil_match_to_ofp10_match(&fr->match, &ofr->match);
        ofr->cookie = fr->cookie;
        ofr->priority = htons(fr->priority);
        ofr->reason = reason;
        ofr->duration_sec = htonl(fr->duration_sec);
        ofr->duration_nsec = htonl(fr->duration_nsec);
        ofr->idle_timeout = htons(fr->idle_timeout);
        ofr->packet_count = htonll(unknown_to_zero(fr->packet_count));
        ofr->byte_count = htonll(unknown_to_zero(fr->byte_count));
        break;
    }

    case OFPUTIL_P_OF10_NXM:
    case OFPUTIL_P_OF10_NXM_TID: {
        struct nx_flow_removed *nfr;
        int match_len;

        msg = ofpraw_alloc_xid(OFPRAW_NXT_FLOW_REMOVED, OFP10_VERSION,
                               htonl(0), NXM_TYPICAL_LEN);
        ofpbuf_put_zeros(msg, sizeof *nfr);
        match_len = nx_put_match(msg, &fr->match, 0, 0);

        nfr = msg->msg;
        nfr->cookie = fr->cookie;
        nfr->priority = htons(fr->priority);
        nfr->reason = reason;
        nfr->table_id = fr->table_id + 1;
        nfr->duration_sec = htonl(fr->duration_sec);
        nfr->duration_nsec = htonl(fr->duration_nsec);
        nfr->idle_timeout = htons(fr->idle_timeout);
        nfr->match_len = htons(match_len);
        nfr->packet_count = htonll(fr->packet_count);
        nfr->byte_count = htonll(fr->byte_count);
        break;
    }

    default:
        OVS_NOT_REACHED();
    }

    return msg;
}
