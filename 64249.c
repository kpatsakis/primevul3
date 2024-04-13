handle_nxt_set_async_config(struct ofconn *ofconn, const struct ofp_header *oh)
{
    struct ofputil_async_cfg basis = ofconn_get_async_config(ofconn);
    struct ofputil_async_cfg ac;
    enum ofperr error;

    error = ofputil_decode_set_async_config(oh, false, &basis, &ac);
    if (error) {
        return error;
    }

    ofconn_set_async_config(ofconn, &ac);
    if (ofconn_get_type(ofconn) == OFCONN_SERVICE &&
        !ofconn_get_miss_send_len(ofconn)) {
        ofconn_set_miss_send_len(ofconn, OFP_DEFAULT_MISS_SEND_LEN);
    }

    return 0;
}
