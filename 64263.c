handle_set_config(struct ofconn *ofconn, const struct ofp_header *oh)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ofputil_switch_config config;
    enum ofperr error;

    error = ofputil_decode_set_config(oh, &config);
    if (error) {
        return error;
    }

    if (ofconn_get_type(ofconn) != OFCONN_PRIMARY
        || ofconn_get_role(ofconn) != OFPCR12_ROLE_SLAVE) {
        enum ofputil_frag_handling cur = ofproto->frag_handling;
        enum ofputil_frag_handling next = config.frag;

        if (cur != next) {
            if (ofproto->ofproto_class->set_frag_handling(ofproto, next)) {
                ofproto->frag_handling = next;
            } else {
                VLOG_WARN_RL(&rl, "%s: unsupported fragment handling mode %s",
                             ofproto->name,
                             ofputil_frag_handling_to_string(next));
            }
        }
    }

    if (config.invalid_ttl_to_controller >= 0) {
        ofconn_set_invalid_ttl_to_controller(ofconn,
                                             config.invalid_ttl_to_controller);
    }

    ofconn_set_miss_send_len(ofconn, config.miss_send_len);

    return 0;
}
