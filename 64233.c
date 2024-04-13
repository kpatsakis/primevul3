handle_barrier_request(struct ofconn *ofconn, const struct ofp_header *oh)
{
    struct ofpbuf *buf;

    buf = ofpraw_alloc_reply((oh->version == OFP10_VERSION
                              ? OFPRAW_OFPT10_BARRIER_REPLY
                              : OFPRAW_OFPT11_BARRIER_REPLY), oh, 0);
    ofconn_send_reply(ofconn, buf);
    return 0;
}
