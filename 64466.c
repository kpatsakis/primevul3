ofputil_decode_bundle_ctrl(const struct ofp_header *oh,
                           struct ofputil_bundle_ctrl_msg *msg)
{
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    enum ofpraw raw = ofpraw_pull_assert(&b);
    ovs_assert(raw == OFPRAW_OFPT14_BUNDLE_CONTROL
               || raw == OFPRAW_ONFT13_BUNDLE_CONTROL);

    const struct ofp14_bundle_ctrl_msg *m = b.msg;
    msg->bundle_id = ntohl(m->bundle_id);
    msg->type = ntohs(m->type);
    msg->flags = ntohs(m->flags);

    return 0;
}
