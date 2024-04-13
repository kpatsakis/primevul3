handle_ipfix_flow_stats_request(struct ofconn *ofconn,
                                const struct ofp_header *request)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ovs_list replies;
    enum ofperr error;

    ofpmp_init(&replies, request);
    error = ofproto_get_ipfix_stats(ofproto, false, &replies);

    if (!error) {
        ofconn_send_replies(ofconn, &replies);
    } else {
        ofpbuf_list_delete(&replies);
    }

    return error;
}
