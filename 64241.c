handle_meter_features_request(struct ofconn *ofconn,
                              const struct ofp_header *request)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ofputil_meter_features features;
    struct ofpbuf *b;

    if (ofproto->ofproto_class->meter_get_features) {
        ofproto->ofproto_class->meter_get_features(ofproto, &features);
    } else {
        memset(&features, 0, sizeof features);
    }
    b = ofputil_encode_meter_features_reply(&features, request);

    ofconn_send_reply(ofconn, b);
    return 0;
}
