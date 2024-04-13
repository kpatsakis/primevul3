handle_port_request(struct ofconn *ofconn,
                    const struct ofp_header *request, ofp_port_t port_no,
                    void (*cb)(struct ofport *, struct ovs_list *replies))
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ofport *port;
    struct ovs_list replies;

    ofpmp_init(&replies, request);
    if (port_no != OFPP_ANY) {
        port = ofproto_get_port(ofproto, port_no);
        if (port) {
            cb(port, &replies);
        }
    } else {
        HMAP_FOR_EACH (port, hmap_node, &ofproto->ports) {
            cb(port, &replies);
        }
    }

    ofconn_send_replies(ofconn, &replies);
}
