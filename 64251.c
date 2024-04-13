handle_nxt_set_flow_format(struct ofconn *ofconn, const struct ofp_header *oh)
{
    const struct nx_set_flow_format *msg = ofpmsg_body(oh);
    enum ofputil_protocol cur, next;
    enum ofputil_protocol next_base;

    next_base = ofputil_nx_flow_format_to_protocol(ntohl(msg->format));
    if (!next_base) {
        return OFPERR_OFPBRC_EPERM;
    }

    cur = ofconn_get_protocol(ofconn);
    next = ofputil_protocol_set_base(cur, next_base);
    ofconn_set_protocol(ofconn, next);

    return 0;
}
