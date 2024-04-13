ofputil_encode_nx_packet_in(const struct ofputil_packet_in *pin,
                            enum ofp_version version)
{
    struct nx_packet_in *npi;
    struct ofpbuf *msg;
    size_t match_len;

    /* The final argument is just an estimate of the space required. */
    msg = ofpraw_alloc_xid(OFPRAW_NXT_PACKET_IN, version,
                           htonl(0), NXM_TYPICAL_LEN + 2 + pin->packet_len);
    ofpbuf_put_zeros(msg, sizeof *npi);
    match_len = nx_put_match(msg, &pin->flow_metadata, 0, 0);
    ofpbuf_put_zeros(msg, 2);

    npi = msg->msg;
    npi->buffer_id = htonl(UINT32_MAX);
    npi->total_len = htons(pin->packet_len);
    npi->reason = encode_packet_in_reason(pin->reason, version);
    npi->table_id = pin->table_id;
    npi->cookie = pin->cookie;
    npi->match_len = htons(match_len);

    return msg;
}
