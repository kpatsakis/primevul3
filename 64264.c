handle_table_desc_request(struct ofconn *ofconn,
                          const struct ofp_header *request)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ofputil_table_desc *table_desc;
    struct ovs_list replies;
    size_t i;

    query_tables_desc(ofproto, &table_desc);
    ofpmp_init(&replies, request);
    for (i = 0; i < ofproto->n_tables; i++) {
        if (!(ofproto->tables[i].flags & OFTABLE_HIDDEN)) {
            ofputil_append_table_desc_reply(&table_desc[i], &replies,
                                            request->version);
        }
    }
    ofconn_send_replies(ofconn, &replies);
    free(table_desc);
    return 0;
}
