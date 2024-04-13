handle_group_features_stats_request(struct ofconn *ofconn,
                                    const struct ofp_header *request)
{
    struct ofproto *p = ofconn_get_ofproto(ofconn);
    struct ofpbuf *msg;

    msg = ofputil_encode_group_features_reply(&p->ogf, request);
    if (msg) {
        ofconn_send_reply(ofconn, msg);
    }

    return 0;
}
