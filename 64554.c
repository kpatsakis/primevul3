ofputil_encode_ofp12_packet_in(const struct ofputil_packet_in *pin,
                               enum ofp_version version)
{
    enum ofpraw raw = (version >= OFP13_VERSION
                       ? OFPRAW_OFPT13_PACKET_IN
                       : OFPRAW_OFPT12_PACKET_IN);
    struct ofpbuf *msg;

    /* The final argument is just an estimate of the space required. */
    msg = ofpraw_alloc_xid(raw, version,
                           htonl(0), NXM_TYPICAL_LEN + 2 + pin->packet_len);

    struct ofp12_packet_in *opi = ofpbuf_put_zeros(msg, sizeof *opi);
    opi->buffer_id = htonl(UINT32_MAX);
    opi->total_len = htons(pin->packet_len);
    opi->reason = encode_packet_in_reason(pin->reason, version);
    opi->table_id = pin->table_id;

    if (version >= OFP13_VERSION) {
        ovs_be64 cookie = pin->cookie;
        ofpbuf_put(msg, &cookie, sizeof cookie);
    }

    oxm_put_match(msg, &pin->flow_metadata, version);
    ofpbuf_put_zeros(msg, 2);

    return msg;
}
