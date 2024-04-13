handle_table_stats_request(struct ofconn *ofconn,
                           const struct ofp_header *request)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ofputil_table_features *features;
    struct ofputil_table_stats *stats;
    struct ofpbuf *reply;
    size_t i;

    query_tables(ofproto, &features, &stats);

    reply = ofputil_encode_table_stats_reply(request);
    for (i = 0; i < ofproto->n_tables; i++) {
        if (!(ofproto->tables[i].flags & OFTABLE_HIDDEN)) {
            ofputil_append_table_stats_reply(reply, &stats[i], &features[i]);
        }
    }
    ofconn_send_reply(ofconn, reply);

    free(features);
    free(stats);

    return 0;
}
