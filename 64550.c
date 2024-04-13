ofputil_encode_ofp10_packet_in(const struct ofputil_packet_in *pin)
{
    struct ofp10_packet_in *opi;
    struct ofpbuf *msg;

    msg = ofpraw_alloc_xid(OFPRAW_OFPT10_PACKET_IN, OFP10_VERSION,
                           htonl(0), pin->packet_len);
    opi = ofpbuf_put_zeros(msg, offsetof(struct ofp10_packet_in, data));
    opi->total_len = htons(pin->packet_len);
    opi->in_port = htons(ofp_to_u16(pin->flow_metadata.flow.in_port.ofp_port));
    opi->reason = encode_packet_in_reason(pin->reason, OFP10_VERSION);
    opi->buffer_id = htonl(UINT32_MAX);

    return msg;
}
