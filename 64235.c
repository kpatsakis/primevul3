handle_features_request(struct ofconn *ofconn, const struct ofp_header *oh)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ofputil_switch_features features;
    struct ofport *port;
    bool arp_match_ip;
    struct ofpbuf *b;

    query_switch_features(ofproto, &arp_match_ip, &features.ofpacts);

    features.datapath_id = ofproto->datapath_id;
    features.n_buffers = 0;
    features.n_tables = ofproto_get_n_visible_tables(ofproto);
    features.capabilities = (OFPUTIL_C_FLOW_STATS | OFPUTIL_C_TABLE_STATS |
                             OFPUTIL_C_PORT_STATS | OFPUTIL_C_QUEUE_STATS |
                             OFPUTIL_C_GROUP_STATS | OFPUTIL_C_BUNDLES);
    if (arp_match_ip) {
        features.capabilities |= OFPUTIL_C_ARP_MATCH_IP;
    }
    /* FIXME: Fill in proper features.auxiliary_id for auxiliary connections */
    features.auxiliary_id = 0;
    b = ofputil_encode_switch_features(&features, ofconn_get_protocol(ofconn),
                                       oh->xid);
    HMAP_FOR_EACH (port, hmap_node, &ofproto->ports) {
        ofputil_put_switch_features_port(&port->pp, b);
    }

    ofconn_send_reply(ofconn, b);
    return 0;
}
