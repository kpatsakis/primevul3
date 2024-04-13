ofputil_put_packet_in(const struct ofputil_packet_in *pin,
                      enum ofp_version version, size_t include_bytes,
                      struct ofpbuf *msg)
{
    /* Add packet properties. */
    ofpprop_put(msg, NXPINT_PACKET, pin->packet, include_bytes);
    if (include_bytes != pin->packet_len) {
        ofpprop_put_u32(msg, NXPINT_FULL_LEN, pin->packet_len);
    }

    /* Add flow properties. */
    ofpprop_put_u8(msg, NXPINT_TABLE_ID, pin->table_id);
    if (pin->cookie != OVS_BE64_MAX) {
        ofpprop_put_be64(msg, NXPINT_COOKIE, pin->cookie);
    }

    /* Add other properties. */
    ofpprop_put_u8(msg, NXPINT_REASON,
                   encode_packet_in_reason(pin->reason, version));

    size_t start = ofpprop_start(msg, NXPINT_METADATA);
    oxm_put_raw(msg, &pin->flow_metadata, version);
    ofpprop_end(msg, start);
}
