handle_get_config_request(struct ofconn *ofconn, const struct ofp_header *oh)
{
    struct ofputil_switch_config config;
    config.frag = ofconn_get_ofproto(ofconn)->frag_handling;
    config.invalid_ttl_to_controller
        = ofconn_get_invalid_ttl_to_controller(ofconn);
    config.miss_send_len = ofconn_get_miss_send_len(ofconn);

    ofconn_send_reply(ofconn, ofputil_encode_get_config_reply(oh, &config));

    return 0;
}
