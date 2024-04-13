handle_queue_get_config_request(struct ofconn *ofconn,
                                const struct ofp_header *oh)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ovs_list replies;
    struct ofport *port;
    ofp_port_t req_port;
    uint32_t req_queue;
    enum ofperr error;

    error = ofputil_decode_queue_get_config_request(oh, &req_port, &req_queue);
    if (error) {
        return error;
    }

    ofputil_start_queue_get_config_reply(oh, &replies);
    if (req_port == OFPP_ANY) {
        error = OFPERR_OFPQOFC_BAD_QUEUE;
        HMAP_FOR_EACH (port, hmap_node, &ofproto->ports) {
            if (!handle_queue_get_config_request_for_port(port, req_queue,
                                                          &replies)) {
                error = 0;
            }
        }
    } else {
        port = ofproto_get_port(ofproto, req_port);
        error = (port
                 ? handle_queue_get_config_request_for_port(port, req_queue,
                                                            &replies)
                 : OFPERR_OFPQOFC_BAD_PORT);
    }
    if (!error) {
        ofconn_send_replies(ofconn, &replies);
    } else {
        ofpbuf_list_delete(&replies);
    }

    return error;
}
