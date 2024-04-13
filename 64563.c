ofputil_encode_resume(const struct ofputil_packet_in *pin,
                      const struct ofpbuf *continuation,
                      enum ofputil_protocol protocol)
{
    enum ofp_version version = ofputil_protocol_to_ofp_version(protocol);
    size_t extra = pin->packet_len + NXM_TYPICAL_LEN + continuation->size;
    struct ofpbuf *msg = ofpraw_alloc_xid(OFPRAW_NXT_RESUME, version,
                                          0, extra);
    ofputil_put_packet_in(pin, version, pin->packet_len, msg);
    ofpprop_put_nested(msg, NXPINT_CONTINUATION, continuation);
    ofpmsg_update_length(msg);
    return msg;
}
