handle_table_features_request(struct ofconn *ofconn,
                              const struct ofp_header *request)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ofpbuf msg = ofpbuf_const_initializer(request,
                                                 ntohs(request->length));
    ofpraw_pull_assert(&msg);
    if (msg.size || ofpmp_more(request)) {
        return OFPERR_OFPTFFC_EPERM;
    }

    struct ofputil_table_features *features;
    query_tables(ofproto, &features, NULL);

    struct ovs_list replies;
    ofpmp_init(&replies, request);
    for (size_t i = 0; i < ofproto->n_tables; i++) {
        if (!(ofproto->tables[i].flags & OFTABLE_HIDDEN)) {
            ofputil_append_table_features_reply(&features[i], &replies);
        }
    }
    ofconn_send_replies(ofconn, &replies);

    free(features);

    return 0;
}
