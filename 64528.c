ofputil_encode_bundle_add(enum ofp_version ofp_version,
                          struct ofputil_bundle_add_msg *msg)
{
    struct ofpbuf *request;
    struct ofp14_bundle_ctrl_msg *m;

    /* Must use the same xid as the embedded message. */
    request = ofpraw_alloc_xid(ofp_version == OFP13_VERSION
                               ? OFPRAW_ONFT13_BUNDLE_ADD_MESSAGE
                               : OFPRAW_OFPT14_BUNDLE_ADD_MESSAGE, ofp_version,
                               msg->msg->xid, ntohs(msg->msg->length));
    m = ofpbuf_put_zeros(request, sizeof *m);

    m->bundle_id = htonl(msg->bundle_id);
    m->flags = htons(msg->flags);
    ofpbuf_put(request, msg->msg, ntohs(msg->msg->length));

    ofpmsg_update_length(request);
    return request;
}
