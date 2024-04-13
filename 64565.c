ofputil_encode_set_async_config(const struct ofputil_async_cfg *ac,
                                uint32_t oams, enum ofp_version ofp_version)
{
    enum ofpraw raw = (ofp_version >= OFP14_VERSION ? OFPRAW_OFPT14_SET_ASYNC
                       : oams & OAM_EXTENSIONS ? OFPRAW_NXT_SET_ASYNC_CONFIG2
                       : ofp_version >= OFP13_VERSION ? OFPRAW_OFPT13_SET_ASYNC
                       : OFPRAW_NXT_SET_ASYNC_CONFIG);
    struct ofpbuf *request = ofpraw_alloc(raw, ofp_version, 0);
    ofputil_put_async_config__(ac, request,
                               (raw == OFPRAW_OFPT14_SET_ASYNC ||
                                raw == OFPRAW_NXT_SET_ASYNC_CONFIG2),
                               ofp_version, oams);
    return request;
}
