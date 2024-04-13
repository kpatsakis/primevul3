ofputil_encode_nx_packet_in2(const struct ofputil_packet_in_private *pin,
                             enum ofp_version version, size_t include_bytes)
{
    /* 'extra' is just an estimate of the space required. */
    size_t extra = (pin->public.packet_len
                    + NXM_TYPICAL_LEN   /* flow_metadata */
                    + pin->stack_size * 4
                    + pin->actions_len
                    + pin->action_set_len
                    + 256);     /* fudge factor */
    struct ofpbuf *msg = ofpraw_alloc_xid(OFPRAW_NXT_PACKET_IN2, version,
                                          htonl(0), extra);

    ofputil_put_packet_in_private(pin, version, include_bytes, msg);
    if (pin->public.userdata_len) {
        ofpprop_put(msg, NXPINT_USERDATA, pin->public.userdata,
                    pin->public.userdata_len);
    }

    ofpmsg_update_length(msg);
    return msg;
}
