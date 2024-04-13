ofputil_encode_flow_stats_request(const struct ofputil_flow_stats_request *fsr,
                                  enum ofputil_protocol protocol)
{
    struct ofpbuf *msg;
    enum ofpraw raw;

    switch (protocol) {
    case OFPUTIL_P_OF11_STD:
    case OFPUTIL_P_OF12_OXM:
    case OFPUTIL_P_OF13_OXM:
    case OFPUTIL_P_OF14_OXM:
    case OFPUTIL_P_OF15_OXM:
    case OFPUTIL_P_OF16_OXM: {
        struct ofp11_flow_stats_request *ofsr;

        raw = (fsr->aggregate
               ? OFPRAW_OFPST11_AGGREGATE_REQUEST
               : OFPRAW_OFPST11_FLOW_REQUEST);
        msg = ofpraw_alloc(raw, ofputil_protocol_to_ofp_version(protocol),
                           ofputil_match_typical_len(protocol));
        ofsr = ofpbuf_put_zeros(msg, sizeof *ofsr);
        ofsr->table_id = fsr->table_id;
        ofsr->out_port = ofputil_port_to_ofp11(fsr->out_port);
        ofsr->out_group = htonl(fsr->out_group);
        ofsr->cookie = fsr->cookie;
        ofsr->cookie_mask = fsr->cookie_mask;
        ofputil_put_ofp11_match(msg, &fsr->match, protocol);
        break;
    }

    case OFPUTIL_P_OF10_STD:
    case OFPUTIL_P_OF10_STD_TID: {
        struct ofp10_flow_stats_request *ofsr;

        raw = (fsr->aggregate
               ? OFPRAW_OFPST10_AGGREGATE_REQUEST
               : OFPRAW_OFPST10_FLOW_REQUEST);
        msg = ofpraw_alloc(raw, OFP10_VERSION, 0);
        ofsr = ofpbuf_put_zeros(msg, sizeof *ofsr);
        ofputil_match_to_ofp10_match(&fsr->match, &ofsr->match);
        ofsr->table_id = fsr->table_id;
        ofsr->out_port = htons(ofp_to_u16(fsr->out_port));
        break;
    }

    case OFPUTIL_P_OF10_NXM:
    case OFPUTIL_P_OF10_NXM_TID: {
        struct nx_flow_stats_request *nfsr;
        int match_len;

        raw = (fsr->aggregate
               ? OFPRAW_NXST_AGGREGATE_REQUEST
               : OFPRAW_NXST_FLOW_REQUEST);
        msg = ofpraw_alloc(raw, OFP10_VERSION, NXM_TYPICAL_LEN);
        ofpbuf_put_zeros(msg, sizeof *nfsr);
        match_len = nx_put_match(msg, &fsr->match,
                                 fsr->cookie, fsr->cookie_mask);

        nfsr = msg->msg;
        nfsr->out_port = htons(ofp_to_u16(fsr->out_port));
        nfsr->match_len = htons(match_len);
        nfsr->table_id = fsr->table_id;
        break;
    }

    default:
        OVS_NOT_REACHED();
    }

    return msg;
}
