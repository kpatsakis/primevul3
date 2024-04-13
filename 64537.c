ofputil_encode_get_async_reply(const struct ofp_header *oh,
                               const struct ofputil_async_cfg *ac)
{
    struct ofpbuf *buf;

    enum ofpraw raw = (oh->version < OFP14_VERSION
                       ? OFPRAW_OFPT13_GET_ASYNC_REPLY
                       : OFPRAW_OFPT14_GET_ASYNC_REPLY);
    struct ofpbuf *reply = ofpraw_alloc_reply(raw, oh, 0);
    ofputil_put_async_config__(ac, reply,
                               raw == OFPRAW_OFPT14_GET_ASYNC_REPLY,
                               oh->version, UINT32_MAX);
    return reply;

    return buf;
}
