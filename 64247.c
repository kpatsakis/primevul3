handle_nxt_get_async_request(struct ofconn *ofconn, const struct ofp_header *oh)
{
    struct ofputil_async_cfg ac = ofconn_get_async_config(ofconn);
    ofconn_send_reply(ofconn, ofputil_encode_get_async_reply(oh, &ac));

    return 0;
}
