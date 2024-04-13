ofputil_encode_bundle_ctrl_reply(const struct ofp_header *oh,
                                 struct ofputil_bundle_ctrl_msg *msg)
{
    struct ofpbuf *buf;
    struct ofp14_bundle_ctrl_msg *m;

    buf = ofpraw_alloc_reply(oh->version == OFP13_VERSION
                             ? OFPRAW_ONFT13_BUNDLE_CONTROL
                             : OFPRAW_OFPT14_BUNDLE_CONTROL, oh, 0);
    m = ofpbuf_put_zeros(buf, sizeof *m);

    m->bundle_id = htonl(msg->bundle_id);
    m->type = htons(msg->type);
    m->flags = htons(msg->flags);

    return buf;
}
