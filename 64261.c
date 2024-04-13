handle_queue_stats_request(struct ofconn *ofconn,
                           const struct ofp_header *rq)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct queue_stats_cbdata cbdata;
    struct ofport *port;
    enum ofperr error;
    struct ofputil_queue_stats_request oqsr;

    COVERAGE_INC(ofproto_queue_req);

    ofpmp_init(&cbdata.replies, rq);
    cbdata.now = time_msec();

    error = ofputil_decode_queue_stats_request(rq, &oqsr);
    if (error) {
        return error;
    }

    if (oqsr.port_no == OFPP_ANY) {
        error = OFPERR_OFPQOFC_BAD_QUEUE;
        HMAP_FOR_EACH (port, hmap_node, &ofproto->ports) {
            if (!handle_queue_stats_for_port(port, oqsr.queue_id, &cbdata)) {
                error = 0;
            }
        }
    } else {
        port = ofproto_get_port(ofproto, oqsr.port_no);
        error = (port
                 ? handle_queue_stats_for_port(port, oqsr.queue_id, &cbdata)
                 : OFPERR_OFPQOFC_BAD_PORT);
    }
    if (!error) {
        ofconn_send_replies(ofconn, &cbdata.replies);
    } else {
        ofpbuf_list_delete(&cbdata.replies);
    }

    return error;
}
